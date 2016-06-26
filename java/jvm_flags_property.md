
线上jvm参数分析

### jvm参数及分析


	CATALINA_OPTS="-server"
	CATALINA_OPTS="${CATALINA_OPTS} -Xms4g -Xmx4g"
	CATALINA_OPTS="${CATALINA_OPTS} -XX:PermSize=256m -XX:MaxPermSize=256m"
	CATALINA_OPTS="${CATALINA_OPTS} -Xmn2g"
	CATALINA_OPTS="${CATALINA_OPTS} -XX:MaxDirectMemorySize=1g"
	CATALINA_OPTS="${CATALINA_OPTS} -XX:SurvivorRatio=10"
	CATALINA_OPTS="${CATALINA_OPTS} -XX:+UseConcMarkSweepGC -XX:+UseCMSCompactAtFullCollection -XX:CMSMaxAbortablePrecleanTime=5000"
	CATALINA_OPTS="${CATALINA_OPTS} -XX:+CMSClassUnloadingEnabled -XX:CMSInitiatingOccupancyFraction=80 -XX:+UseCMSInitiatingOccupancyOnly"
	CATALINA_OPTS="${CATALINA_OPTS} -XX:+ExplicitGCInvokesConcurrent -Dsun.rmi.dgc.server.gcInterval=2592000000 -Dsun.rmi.dgc.client.gcInterval=2592000000"
	CATALINA_OPTS="${CATALINA_OPTS} -XX:ParallelGCThreads=${CPU_COUNT}"
	CATALINA_OPTS="${CATALINA_OPTS} -Xloggc:${MIDDLEWARE_LOGS}/gc.log -XX:+PrintGCDetails -XX:+PrintGCDateStamps"
	CATALINA_OPTS="${CATALINA_OPTS} -XX:+HeapDumpOnOutOfMemoryError -XX:HeapDumpPath=${MIDDLEWARE_LOGS}/java.hprof"
	CATALINA_OPTS="${CATALINA_OPTS} -Djava.awt.headless=true"
	CATALINA_OPTS="${CATALINA_OPTS} -Dsun.net.client.defaultConnectTimeout=10000"
	CATALINA_OPTS="${CATALINA_OPTS} -Dsun.net.client.defaultReadTimeout=30000"
	CATALINA_OPTS="${CATALINA_OPTS} -DJM.LOG.PATH=${MIDDLEWARE_LOGS}"
	CATALINA_OPTS="${CATALINA_OPTS} -DJM.SNAPSHOT.PATH=${MIDDLEWARE_SNAPSHOTS}"
	CATALINA_OPTS="${CATALINA_OPTS} -Dfile.encoding=${JAVA_FILE_ENCODING}"
	CATALINA_OPTS="${CATALINA_OPTS} -Dhsf.publish.delayed=true"

一个一个来解释

	CATALINA_OPTS="-server"

client模式的特点是启动快、占用内存少、JIT编译器生成代码的速度也更快。Server模式则提供了更复杂的生成码优化功能，这个功能对于服务器程序而言尤为重要。大多数server模式的的JIT编译优化都要消耗额外的时间一手机更多的应用程序行为信息、为用用程序运行生成更优的代码。


	CATALINA_OPTS="${CATALINA_OPTS} -Xms4g -Xmx4g"

设置新生代和老年代的总大小，关注吞吐量的及延迟的java应用程序应该将-Xms和-xmx设定为一个值。这是因为无论扩展还是缩减新生代空间和老年代空间都需要进行full gc，而full gc会降低程序的吞吐量并导致更长的延迟。


	CATALINA_OPTS="${CATALINA_OPTS} -XX:PermSize=256m -XX:MaxPermSize=256m"

设置了永久代的大小，一般将永久带的大小的初始值和最大值设置为同一值，因为永久代空间的大小调整需要进行full gc才能实现。

	CATALINA_OPTS="${CATALINA_OPTS} -Xmn2g"

新生代的初始值和最大值为2g

	CATALINA_OPTS="${CATALINA_OPTS} -XX:MaxDirectMemorySize=1g"

设置了java NIO直接分配的堆外内存的为1g

	CATALINA_OPTS="${CATALINA_OPTS} -XX:SurvivorRatio=10"

新生代的surivior和eden的比例为1:10, so 因为有两个surivior，一个surivior的大小为1/12 * 2g = 166M

	CATALINA_OPTS="${CATALINA_OPTS} -XX:+UseConcMarkSweepGC -XX:+UseCMSCompactAtFullCollection -XX:CMSMaxAbortablePrecleanTime=5000"

-XX:UseConcMarkSweepGC,老年代使用CMS回收器，会自动开启UseParNewGC选线，新生代使用ParNew垃圾回收器  
-XX:+UseCMSCompactAtFullCollection, [使用跟Serial Old GC一样的LISP2算法的mark-compact来做full GC][0], 这个选项在jdk9会被移除掉， [在jdk8中被标识为了Deprecated][4]  
[-XX:CMSMaxAbortablePrecleanTime][1]，限制CMS-concurrent-abortable-preclean的时间为5s, 这个貌似也是默认值。

	CATALINA_OPTS="${CATALINA_OPTS} -XX:+CMSClassUnloadingEnabled -XX:CMSInitiatingOccupancyFraction=80 -XX:+UseCMSInitiatingOccupancyOnly"

-XX:CMSClassUnloadingEnabled 对perm区不使用的class进行回收。   
-XX:CMSInitiatingOccupancyFraction=80 在老年代的空间占用到达80%的时候，启动CMS回收。  
-XX:+UseCMSInitiatingOccupancyOnly 如果没有这个选项，会在第一次老茧回收使用-XX:CMSInitiatingOccupancyFraction的设置，后续JVM会自己估计一个数值。

	CATALINA_OPTS="${CATALINA_OPTS} -XX:+ExplicitGCInvokesConcurrent -Dsun.rmi.dgc.server.gcInterval=2592000000 -Dsun.rmi.dgc.client.gcInterval=2592000000"


-XX:+ExplicitGCInvokesConcurrent 把System.gc转成一个CMS，而不是一个Full GC。  
-Dsun.rmi.dgc.server.gcInterval=2592000000 和 -Dsun.rmi.dgc.client.gcInterval=2592000000是为了减少RMI引起的System.gc的频率。单位是ms。 如果用到了RMI，30天进行一次。我们应该没用到，因为有hsf。

	CATALINA_OPTS="${CATALINA_OPTS} -XX:ParallelGCThreads=${CPU_COUNT}"

设置并发阶段GC的线程的数量， jdk的默认值是如果cpu数量小于等于8, 则为cpu count，我们线上环境一般都是4core的，所以只是把默认值显式的指定了一下。

	CATALINA_OPTS="${CATALINA_OPTS} -Xloggc:${MIDDLEWARE_LOGS}/gc.log -XX:+PrintGCDetails -XX:+PrintGCDateStamps"
	
设置GC路径，打印详细的gc日志和时间戳。


	CATALINA_OPTS="${CATALINA_OPTS} -XX:+HeapDumpOnOutOfMemoryError -XX:HeapDumpPath=${MIDDLEWARE_LOGS}/java.hprof"

如果因为堆内存不够JVM扛不住了，在推出前会在heapDumpPath dump一个文件出来，方便后续分析。

    -Djava.awt.headless=true

Headless mode is a system configuration in which the display device, keyboard, or mouse is lacking.[2] 设置成这种方式，可以让我们在没有显示器、鼠标和键盘的情况下做些图形处理工作。


	CATALINA_OPTS="${CATALINA_OPTS} -Dsun.net.client.defaultConnectTimeout=10000"
	CATALINA_OPTS="${CATALINA_OPTS} -Dsun.net.client.defaultReadTimeout=30000"

设置java.net.URLConnection的建立连接和读数据的默认超时时间。


	CATALINA_OPTS="${CATALINA_OPTS} -DJM.LOG.PATH=${MIDDLEWARE_LOGS}"

设置中间件团队的日志的默认目录，

	CATALINA_OPTS="${CATALINA_OPTS} -DJM.SNAPSHOT.PATH=${MIDDLEWARE_SNAPSHOTS}"

不知是什么鬼？
	
	CATALINA_OPTS="${CATALINA_OPTS} -Dfile.encoding=${JAVA_FILE_ENCODING}"

设置JVM的默认字符编码	
	
	CATALINA_OPTS="${CATALINA_OPTS} -Dhsf.publish.delayed=true"

这是看起来是个延时发布，目的是等应用启动并初始化后再进行hsf服务发布, [通过回调pandroa的api来实现][7], hsf是作为pandroa的一个插件部署的，提供了[以下命令][8], online-hsf其实就是把hsf的服务地址通过configServer发布出去, [具体的过程在这里][9].
	


### jdk8 

升级jdk8后，参数做了些细微[调整][10]

	if [ "$JAVA_VERSION_MAJOR" -ge 8 ]; then
        CATALINA_OPTS="${CATALINA_OPTS//PermSize/MetaspaceSize}"
        CATALINA_OPTS="${CATALINA_OPTS//MaxPermSize/MaxMetaspaceSize}"
        CATALINA_OPTS="${CATALINA_OPTS//-XX:+UseCMSCompactAtFullCollection/}"
        SERVICE_OPTS="${SERVICE_OPTS//PermSize/MetaspaceSize}"
        SERVICE_OPTS="${SERVICE_OPTS//MaxPermSize/MaxMetaspaceSize}"
        SERVICE_OPTS="${SERVICE_OPTS//-XX:+UseCMSCompactAtFullCollection/}"
        export CATALINA_OPTS
    fi


a. 把perm区相关的参数替换了metaspace
b. 移除UseCMSCompactAtFullCollection参数，上文已经提到了。


### 其他参考资料：

1. java性能优化权威指南
2. pe维护启动脚本的[仓库地址][5], jvm选项所在的文件[set-env.sh][6]
3. app.release中ajdk升级规范，[update_setenv.sh][10]
4. Pandora QOS 命令客户端[wiki][11]




[0]: http://hllvm.group.iteye.com/group/topic/42365

[1]: https://blogs.oracle.com/jonthecollector/entry/did_you_know 

[2]: http://www.oracle.com/technetwork/articles/javase/headless-136834.html

[3]: http://docs.oracle.com/javase/7/docs/technotes/guides/net/properties.html

[4]: http://openjdk.java.net/jeps/214

[5]: http://gitlab.alibaba-inc.com/ais-pe-platform/apps-deploy

[6]: http://gitlab.alibaba-inc.com/ais-pe-platform/apps-deploy/blob/master/admin_noweb_tomcat7/appname/bin/setenv.sh

[7]: http://gitlab.alibaba-inc.com/ais-pe-platform/apps-deploy/blob/master/admin_noweb_tomcat7/appname/bin/appctl.sh#L309

[8]: http://gitlab.alibaba-inc.com/middleware/hsf2-0/wikis/pandoracommond

[9]: http://gitlab.alibaba-inc.com/middleware/hsf2-0/blob/master/hsf.service.monitor/src/main/java/com/taobao/hsf/info/pandora/HSFCommandProvider.java#L155

[10]: http://gitlab.alibaba-inc.com/app-release/app-release-specification

[11]: http://gitlab.alibaba-inc.com/middleware-container/pandora-qos-service/wikis/home
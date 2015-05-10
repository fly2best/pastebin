####问题排查: 应用每小时一次的full gc(system.gc)


####起因与初步解决

前段时间，我们的应用会出现每小时一次的full gc。看gc日志是`Systme.gc`引起的。

最初是直接把System.gc disable掉了(`-XX:+DisableExplicitGC`)，后来了解到会造成堆外内存溢出，我们依赖的jar包使用了netty通信框架，netty使用了DirectMemory，为了回收堆外内存会调用`System.gc`, 如果禁用System.gc会造成堆外内存溢出，正确的做法是把System.gc转换成一个CMS(`-XX:+ExplicitGCInvokesConcurrent`). 如此可以避免出现每小时一次的full gc。

####但是这个System.gc是谁调用的呢，为什么要这么做？

google以后，发现有可能有两种原因：

1.  JreMemoryLeakPreventionListener 
    
    这是tomcat的一个bug，可以参考[stackoverflow上的一个提问](http://stackoverflow.com/questions/14902928/why-does-the-jvm-of-these-tomcat-servers-perform-a-full-gc-hourly)。

    
2. RMI 
   
   java rmi为了回收不被引用的remote对象，会定时调用`System.gc`，可以参考[这里](https://plumbr.eu/blog/garbage-collection/rmi-enforcing-full-gc-to-run-hourly).
   
   		"When it is necessary to ensure that unreachable remote objects are unexported and garbage collected in a timely fashion, the value of this property represents the maximum interval (in milliseconds) that the Java RMI runtime will allow between garbage collections of the local heap. The default value is 3600000 milliseconds (one hour)." 
    

#### 定位问题的原因

对于情况1，可以设置把gcDaemonProtection关掉来测试: `<Listener className="org.apache.catalina.core.JreMemoryLeakPreventionListener"
gcDaemonProtection="false"/>`， 我这次遇到的问题是这货引起的，但是我把配置加错位置了，所以一开始没有正确定位原因，走了一条相对曲折了路才正确定位到原因。

对于情况2， [可以设置以下两个参数来影响rmi gc的间隔](http://www-01.ibm.com/support/docview.wss?uid=swg21173431)，试了一下没啥用。

	-Dsun.rmi.dgc.server.gcInterval=VALUE
	-Dsun.rmi.dgc.client.gcInterval=VALUE

陷入僵局，不知道该怎么弄。

####上btrace
用btrace脚本去看`System.gc`去定位调用栈， 得到的结果如下：
    
    java.lang.System.gc(System.java:926)
	sun.misc.GC$Daemon.run(GC.java:92	
	
查资料说sun.misc.GC$Daemon是个系统进程，我就把这货给忽略了，觉得从这里定位不到原因，就忽略了，真是个马大哈啊，没有认真看sun.misc.GC的代码也没有认真思考。

仔细看过`sun.msic.GC`的代码后，发现[在调用`sun.misc.GC.setLatencyTarget`时会导致Dameno进程的创建。](http://grepcode.com/file/repository.grepcode.com/java/root/jdk/openjdk/6-b14/sun/misc/GC.java#153)

所以用btrace去跟对这个方法的调用，跌破眼镜，竟然看到了`org.apache.catalina.core.JreMemoryLeakPreventionListener`

	-----sun.misc.GC.setLatencyTarget----jstack----
   	sun.misc.GC.setLatencyTarget(GC.java)
   	sun.misc.GC.access$500(GC.java:24)
   	sun.misc.GC$LatencyRequest.adjustLatencyIfNeeded(GC.java:175)
   	sun.misc.GC$LatencyRequest.<init>(GC.java:200)
   	sun.misc.GC$LatencyRequest.<init>(GC.java:156)
   	sun.misc.GC.requestLatency(GC.java:254)
   	sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
   	sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:39)
   	sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:25)
   	java.lang.reflect.Method.invoke(Method.java:597)
   	org.apache.catalina.core.JreMemoryLeakPreventionListener.lifecycleEvent(JreMemoryLeakPreventionListener.java:295)
   	org.apache.catalina.util.LifecycleSupport.fireLifecycleEvent(LifecycleSupport.java:119)
   	org.apache.catalina.util.LifecycleBase.fireLifecycleEvent(LifecycleBase.java:90)
   	org.apache.catalina.util.LifecycleBase.setStateInternal(LifecycleBase.java:401)
   	org.apache.catalina.util.LifecycleBase.init(LifecycleBase.java:99)
   	org.apache.catalina.startup.Catalina.load(Catalina.java:594)
   	org.apache.catalina.startup.Catalina.start(Catalina.java:633)
   	sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
   	sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:39)
   	sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:25)
   	java.lang.reflect.Method.invoke(Method.java:597)
   	org.apache.catalina.startup.Bootstrap.start(Bootstrap.java:322)
   	org.apache.catalina.startup.Bootstrap.main(Bootstrap.java:450)
	-----sun.misc.GC.setLatencyTarget----jstack----

尼玛真是没想到啊，那先前的配置一定是加错地方了，grep了一把，发现还有另外一个配置文件（我们使用的tomcat配置和标准的tomcat有区别，如果是标准tomcat结构就是server.xml). 

这才算是定位了System.gc的起因，按前文提到的文章，把gcDaemonProtection设置为false就可以避免这个问题。但是把这个gcDaeonProtection禁用掉，会引来其他问题吗？

###JreMemoryLeakPreventionListener

JreMemoryLeakPreventionListener是为了避免web应用的classloader初始化一个java运行是环境的单例资源而造成了内存泄露。 如果WebappClassLoader初始化了java runtime的单例资源，但应用重新部署或停止的时候，当前应用的的classLoader会被那个单例资源hold住，进一步导致classLoader引用的其他资源无法被释放，而造成clasLoader泄露。

摘自[tomcat文档](https://tomcat.apache.org/tomcat-7.0-doc/config/listeners.html)

	The JRE Memory Leak Prevention Listener provides work-arounds for known places where the Java Runtime environment uses the context class loader to load a singleton as this will cause a memory leak if a web application class loader happens to be the context class loader at the time. The work-around is to initialise these singletons when this listener starts as Tomcat's common class loader is the context class loader at that time. It also provides work-arounds for known issues that can result in locked JAR files.

如果sun.misc.GC.$Daemon的contextClassLoader是WebappClassLoader则会造成内存泄露，[参考这个demo](http://wiki.apache.org/tomcat/MemoryLeakProtection#cclThreadSpawnedByWebApp), gcDaemonProtection这个选项就是为了避免出现这种情况的，如果gcDaemonProtection被设置为true，则会利用tomcat的common classLoader作为来初始化GC Daemon线程，但是tomcat在最初实现这个功能的时候把latency的值设为了一个1小时，这个值小了，为了避免对应用产生影响，这个值应该尽量大，我们的只是要触发Gc Daemon线程的创建，tomcat的后续版本修复了这个问题，把laetency设置为了Long.MAX_VALUE - 1。

我们用的tomcat版本略低，有这个问题，为了避免这个问题，可以升级tomcat版本或者把gcDaemonProtection禁用掉， 升级生产环境tomcat版本还是要谨慎点。

我们现在生成环境中不会多个web应用共用一个tomcat，一般也不会用tomcat的热部署功能，所以即使GC Daemon线程的contextClassLoader是WebappClassLoader也不会用有多大问题。还有我们应用的代码及其所依赖的库里没有使用到GC Daemon的功能（挂个btrace看调用就能跟出来），因此可以放心的的把GC Daemon给禁用掉。

如此可以完美解决这个这个JreMemoryLeakPreventionListener引起的每小时一个的System.gc问题。


ps: 为了用btrace追踪tomcat的启动，需要挂一个javaagent，btrace提供的，刚开始不知道btrace有这个功能，我去用了JVMTI，最近刚听过同事的分享，能吐槽不改好上手吗？折腾了好久没成功（获取函数调用的参数太麻烦了）。anyway，最后是用了btrace提供的agent跟踪了应用的初始化，相关使用方法：

	JAVA_OPTS="$JAVA_OPTS -javaagent:/path/to/btrace-agent.jar=scriptdir=/path/to/script/class/dir,script=TraceSetLatency.class,scriptOutputFile=/path/to/log/btrace.log,debug=true"

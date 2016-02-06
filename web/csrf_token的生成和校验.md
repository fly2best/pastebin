CSRF Token的生成和校验

什么是CSRF及其攻防，参考一下[浅谈CSRF攻击方式][1], 写的通俗易懂，我就不再啰嗦了。今天主要看看使用token的生成和校验。

### Token的生成和校验
在[浅谈CSRF攻击方式][1]中，提到了集中使用Token来进行攻防的例子。
有几处说的不是特别清楚，我来补充一下。并给出java代码的例子。

google一下能搜到几个CSRFToken的Util

a.  [OWASP CSRF Token Util][2]

    上述代码给出了一个简单的实现，每个session共用一个CSRF Token，生成这个token后放在httpSession中，校验的时候把request的中的token和session中的token比较即可。
    
    如果requst中的cookie为空或者和session中的token不一致校验则不通过，代码如下：
        
        public static boolean isValid (HttpServletRequest request) 
         throws ServletException, NoSuchAlgorithmException {
                    //throw exception if session is null
                    if (request.getSession(false)== null) {
                        throw new ServletException(NO_SESSION_ERROR);
                    }
                    return getToken(request.getSession(false)).equals(
                                request.getParameter(SESSION_ATTR_KEY));
         }

	简单粗暴，但是有效唉。妈妈再也不用担心我被csrf了。。。

b. webx中的实现
    
    webx是啥我就不啰嗦了，我们只看看他的csrf的实现。webx的实现稍显复杂，[CSRF Token的生成类][3], [CSRF Token的校验类][4].  
    webx中给出了两种实现，一种是基于session的长存活token，一是只使用一次的短存活时间token。
    
    除了存活时间长短以外没有本质区别，没有别的区别。不过这个uniqueToken的生成和移除应该不是线程安全的，不知道在实际用的时候有没有问题。在我们的web工程中使用的是loneLive的Token生成和校验方法。
    
    不过web想中校验有坑啊, 如果从requst中拿不到token就不校验了，这个如果攻击者把token给去掉不就可以攻击了吗？
    我们在使用的时候还要自己看看这个token有没有传进来才行。
    
        if (tokenFromRequest != null) {
            HttpSession session = rundata.getRequest().getSession();

            // 先检查longLiveToken，如果匹配，则不用检查uniqueToken了。
            if (!tokenFromRequest.equals(CsrfToken.getLongLiveTokenInSession(session))) {
                List<String> tokensInSession = CsrfToken.getTokensInSession(session, tokenKey);

                if (!tokensInSession.contains(tokenFromRequest)) {
                    // 如果不符则终止请求
                    requestExpired(rundata, tokenFromRequest, tokensInSession);
                } else {
                    // 如果符合，则清除session中相应的token，以防止再次使用它
                    tokensInSession.remove(tokenFromRequest);

                    CsrfToken.setTokensInSession(session, tokenKey, tokensInSession);
                }
            }
        }

先前在YY CSRF Unique Token的使用的时候，我还以为会使用集中存储，后面挂一个memory cache啥的。 实际上没这么复杂，直接维护在session中就可以了。不过并发问题怎么办？假设一个页面上有多个form，应该会导致并发修改同一个session中的token的，对单个session对象加锁？ 反正可以解决，回头我测试一下。

新年快乐~

[1]: http://www.cnblogs.com/hyddd/archive/2009/04/09/1432744.html “浅谈CSRF攻击方式”
[2]: https://code.google.com/p/owasp-esapi-java/issues/attachmentText?id=162&aid=3277549059686427905&name=CSRFTokenUtil.java&token=5d2dc0548ec58bbf6c1cf93e640c94c8
[3]: https://github.com/webx/citrus/blob/master/webx/turbine/src/main/java/com/alibaba/citrus/turbine/util/CsrfToken.java
[4]: https://github.com/webx/citrus/blob/master/webx/turbine/src/main/java/com/alibaba/citrus/turbine/pipeline/valve/CheckCsrfTokenValve.java
#!/usr/bin/env python
import urllib
import httplib2
import re

http = httplib2.Http()
loginUrl = 'http://bbs.whnet.edu.cn/cgi-bin/bbslogin'
body = {'id': '', 'pw': ''}
headers = {'Content-type': 'application/x-www-form-urlencoded'}
response, content = http.request(loginUrl, 'POST', headers=headers, body=urllib.urlencode(body))
lines = content.split('\n')
cookie = {}
for cookieStr in re.findall(".*cookie='(.*)'.*", content):
    key = cookieStr.split('=')[0]
    value = cookieStr.split('=')[1]
    cookie[key] = value

cookieList = ['%s=%s' % (k, v) for k, v in cookie.items()]

cookieToSend = ';'.join(cookieList)
print "cookie: %s" % (cookieToSend)

headers = {'Cookie': cookieToSend}
newMailUrl = 'http://bbs.whnet.edu.cn/cgi-bin/bbsnewmail'
response, content = http.request(newMailUrl, 'GET', headers=headers)

print response
print type(content)
print content.decode('gb2312').encode('utf-8')

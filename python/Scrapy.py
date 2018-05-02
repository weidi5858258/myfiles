'''

下面是简单创建一个工程然后运行的步骤(Ubuntu环境下):
=================================================
1.安装scrapy
pip3 install scrapy
2.选定一个合适的目录,然后创建工程
scrapy startproject TestScrapy
3.进入TestScrapy目录,然后创建ExampleSpider类
scrapy genspider example example.com
4.进入TestScrapy目录,运行爬虫: 
scrapy crawl example(唯一标识名字)
=================================================











ExampleSpider类的说明:
name:		唯一标识名字(代码运行时使用的就是这个唯一标识符)
ExampleSpider:	spider类
start_urls:	爬取url
parse:		response处理
修改start_urls, parse

ExampleSpider类中使用地址的两种方式:
# 请求url第一种方式
start_urls = ['http://httpbin.org/ip']
# 请求url第二种方式,使用这种方式时,第一种方式就不会再生效了
# callback表示由这个类的parse函数解析
def start_requests(self):
	yield scrapy.Request("https://movie.douban.com/top250", callback=self.parse)
callback表示会去回调parse函数

提取页面之前,首先需要把页面抓取下来
提取页面信息的三种方式:
re
bs4
response: spath css

如果抓取页面不成功,可能是User-Agent设置的不对,
需要到setting.py中去设置(默认是注释状态):
USER_AGENT = "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/50.0.2661.94 Safari/537.36"

setting.py中的robots.txt rules规定哪些内容可以被抓取,
哪些内容不允许被抓取,默认ROBOTSTXT_OBEY = True.
True表示不允许随意抓取,False表示可以抓取任何内容.

------------------------------------------------------

shell请求的几种方式:
1.
直接请求url:
scrapy shell url
2.
请求url的同时设置请求信息:
scrapy shell -s name=value url
3.
先设置有关属性值
scrapy shell -s name=value
如:
scrapy shell -s User-Agent="Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/50.0.2661.94 Safari/537.36"
接着导入有关的库
from scrapy import Request
然后进行请求
req = Request(url)
req = Request(url, headers={})
response = fetch(req)
使用上面的方式请求后response对象就有下面的这些函数可以使用了
response.body             response.flags            response.request          response.urljoin
response.body_as_unicode  response.follow           response.selector         response.xpath
response.copy             response.headers          response.status           
response.css              response.meta             response.text             
response.encoding         response.replace          response.url

如:
抓取的内容一般有:某个链接地址,下一页
在网页中如何使用XPath:
首先找到需要抓取的内容,右键选择"检查",
在代码中找到需要抓取的内容,右键选择"Copy",
选择"Copy XPath",然后使用'response.xpath("Copy XPath的内容").extract()'

response.url
response.body
关键是提取元素,提取元素的两个方法:
response.xpath("")
response.css("")

1.
xpath:选取xml文档中的节点或者节点集
表达式				说明
/				根节点
/XXX/ZZZ			根据路径一级一级往下查找节点
//div				查找所有div元素
/XXX//div			查找XXX节点下的所有div元素
*				匹配任意节点元素
/html/body/div[1]		查找body下第一个div节点
//div[@class="XXX"]		查找class属性为XXX的div节点
//@attr				提取attr对应的属性值
text()				提取文本信息
position()			选取第几个节点
last()				选取最后一个节点
starts-with(@attr, strh)    	attr属性值开头为strh的节点
contains(@attr, substr)		attr属性值是否包含substr
|				计算两个节点集
比较运算符			=,!=,<,<=......
算数运算符			+,-,*,div
逻辑运算符			or,and

举例说明:
查找所有的title
response.xpath("//title")
response.xpath("/")
response.xpath("/html").extract()
response.xpath("/html/head/title").extract()
response.xpath("//a")
查找body下所有的a
response.xpath("/html/body//a")
response.xpath("*")
response.xpath("/*")
response.xpath("/html/*")
查找body下所有的节点,节点下的节点不查找
response.xpath("//body/*")
查找body下第一个meta节点
response.xpath("//body/meta[1]")
查找class为hd的div节点
response.xpath("//div[@class='hd']//a/span[1]").extract()
response.xpath("//div[@class='hd']//a/@href").extract()
response.xpath("//div[@class='hd']//a/span[1]/text()").extract()
response.xpath("//div[@class='hd']//a/span[position()=1]/text()").extract()
response.xpath("//div[@class='hd']//a/span[last()]/text()").extract()
response.xpath("//div[starts-with(@class, 't')]").extract()
response.xpath("//div | //li")
response.xpath("//span[@class='rating_num' and text() > 9.4]/text()").extract()

2.
css:
表达式				说明
.classvalue			选择class属性为classvalue的元素
#idvalue			选择id值为idvalue的元素
p				选择所有p元素
div, p				选择所有的div和p元素
div~p				前面有div元素的每个p元素
li span				选择li内部所有span元素
div>p				选择父节点为div的所有p元素
::text/::attr(src)		文本信息/提取某个属性的值

1.发送文字
2.发送表情
3.发送图片
4.发送视频
5.发送地理位置
6.发送名片
7.发送收藏的内容
8.发送文件
9.红包
10.转账
11.语音输入
12.(视频或语音)通话











------------------------------------------------------

































































'''













































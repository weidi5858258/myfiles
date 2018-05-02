# -*- coding: utf-8 -*-
import scrapy

'''

'''


class DoubanSpider(scrapy.Spider):
    name = 'mvinfo'
    # allowed_domains = ['example.com']
    # 请求url第一种方式
    start_urls = ['http://httpbin.org/ip']

    # 请求url第二种方式,使用这种方式时,第一种方式就不会再生效了
    # callback表示由这个类的parse函数解析
    def start_requests(self):
        yield scrapy.Request(
            "https://movie.douban.com/top250",
            callback=self.parse)

    def parse(self, response):
        # print(response.text)
        print(response.url)
        print(response.body)

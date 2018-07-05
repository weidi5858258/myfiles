# -*- coding: utf-8 -*-
import scrapy


class ExampleSpider(scrapy.Spider):
    name = 'example'
    # allowed_domains = ['example.com']
    start_urls = ['http://httpbin.org/get']

    def parse(self, response):
        print(response.text)

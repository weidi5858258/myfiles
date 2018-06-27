# -*- coding: utf-8 -*-

import scrapy

class TencentItem(scrapy.Item):
	positionName = scrapy.Field()
	positionLink = scrapy.Field()
	positionType = scrapy.Field()
	peopleNum = scrapy.Field()
	workLocation = scrapy.Field()
	publishTime = scrapy.Field()



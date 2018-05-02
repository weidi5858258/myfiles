#! /usr/bin/python
# -*- coding:utf-8 -*-

import urllib3
import urllib
import json
import requests
import threading
import time
import sys
import os
import re
from urllib.parse import urlencode

# from bs4 import BeautifulSoup

'''
http://www.500px.tv/#/dashboard/popular/0/1
http = urllib3.PoolManager()
GET的第一种方式
r = http.request('GET', 'url')
GET的第二种方式
r = http.request(
        'GET',
        'url',
        headers = {'X-Something':'value'}
    )
GET的第三种方式（GET, HEAD和DELETE请求）
r = http.request(
        'GET',
        'url',
        field = {'name':'weidi'}
    )
    
POST的第一种方式
r = http.request(
        'POST',
        'url',
        fields = {'name':'weidi'}
    )
POST的第二种方式（POST和PUT请求）
encoding_args = urlencode({'arg':'value'})
url = 'http://......' + encoding_args
r = http.request('POST', url) 
Form data:
    对于PUT和POST请求(request),
    urllib3会自动将字典类型的field
    参数编码成表格类型。
JSON:
    在发起请求时,可以通过定义body
    参数并定义headers的Content-Type
    参数来发送一个已经过编译的JSON数据：
    data = {'attribute':'value'}
    encoding_data = json.dumps(data).encode('utf-8')
    r = http.request(
            'POST',
            'url',
            body = encoding_data,
            headers = {'Content-Type':'application/json'}
        )
Files & binary data:
    使用multipart/form-data编码方式上传文件,
    可以使用和传入Form data数据一样的方法进行,
    并将文件定义为一个元组的形式(file_name,file_data):
    with open('example.txt') as fp:
        file_data = fp.read()
    r = http.request(
            'POST',
            'url',
            field = {'filefield':('example.txt', file_data)}
        )
    文件名(filename)的定义不是严格要求的,
    但是推荐使用,以使得表现得更像浏览器。
    同时，还可以向元组中再增加一个数据来定义文件的MIME类型：
    with open('example.txt') as fp:
        file_data = fp.read()
    r = http.request(
            'POST',
            'url',
            field = {'filefield':('example.txt', file_data, 'text/pain')}
        )
    如果是发送原始二进制数据，
    只要将其定义为body参数即可。
    同时，建议对header的Content-Type参数进行设置：
    with open('example.txt') as fp:
        file_data = fp.read()
    r = http.request(
            'POST',
            'url',
            body = binary_data,
            header = {'Content-Type':'image/jpeg'}
        )
Timeout :
    使用timeout，可以控制请求的运行时间。
    在一些简单的应用中，可以将timeout参数设置为一个浮点数：
    r = http.request('GET', 'url', timeout = 3.0)
    要进行更精细的控制，可以使用Timeout实例，
    将连接的timeout和读的timeout分开设置：
    r = http.request('GET', 'url', timeout = urllib2.Timeout(connect = 1.0, read = 2.0))
    如果想让所有的request都遵循一个timeout，
    可以将timeout参数定义在PoolManager中：
    http = urllib3.PoolManager(timeout = 3.0)
    或者
    http = urllib3.PoolManager(timeout = urllib3.Timeout(connect = 1.0, read = 2.0))
    当在具体的request中再次定义timeout时，会覆盖PoolManager层面上的timeout。
请求重试(retrying requests):
    Urllib3 可以自动重试幂等请求，
    原理和handles redirect一样。
    可以通过设置retries参数对重试进行控制。
    Urllib3默认进行3次请求重试，并进行3次方向改变。
    给retries参数定义一个整型来改变请求重试的次数：
    r = http.request('GET', 'url', retries = 10)
    关闭请求重试(retrying request)及重定向(redirect)只要将retries定义为False即可：
    r = http.request('GET', 'url', retries = False)
    关闭重定向(redirect)但保持重试(retrying request),将redirect参数定义为False即可：
    r = http.request('GET', 'url', redirect = False)
    要进行更精细的控制，可以使用retries实例，通过该实例可以对请求的重试进行更精细的控制。
    例如，进行3次请求重试，但是只进行2次重定向：
    r = http.request('GET', 'url', retries = urllib3.Retry(3, redirect = 2))
    如果想让所有请求都遵循一个retry策略，可以在PoolManager中定义retry参数：
    http = urllib3.PoolManager(retries = False)
    或者
    http = urllib3.PoolManager(retries = urllib3.Retry(5, redirect = 2))
    当在具体的request中再次定义retry时，会覆盖 PoolManager层面上的retry。
'''


# headers = {
#     "Origin": "http://www.500px.tv/",
#     "Referer": "http://www.500px.tv/",
#     "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.186 Safari/537.36",
#     'only': 'All',
#     'page': '1',
#     'rpp': '30'
# }
#
# form_data = {
#
# }


# result = requests.get("http://www.500px.tv/#/dashboard/popular/0/1", params=params)
# print(result)

def saveImage2(imageUrl, savePath, imageName, mode):
    if not os.path.exists(savePath):
        os.makedirs(savePath)

    response = requests.get(imageUrl)
    if 'win' in sys.platform:
        saveImagePath = savePath + '\\' + imageName
    else:
        saveImagePath = savePath + '/' + imageName
    with open(saveImagePath, mode) as file:
        file.write(response.content)


def saveFile(fileUrl, savePath, fileName, mode):
    '''
    当使用requests的get下载大文件/数据时，
    建议使用使用stream模式。

    当把get函数的stream参数设置成False时，
    它会立即开始下载文件并放到内存中，
    如果文件过大，有可能导致内存不足。

    当把get函数的stream参数设置成True时，
    它不会立即开始下载，当你使用
    iter_content或iter_lines
    遍历内容或访问内容属性时才开始下载。
    需要注意一点：文件没有下载之前，它也需要保持连接。

    iter_content：一块一块的遍历要下载的内容
    iter_lines：一行一行的遍历要下载的内容
    使用上面两个函数下载大文件可以防止占用过多的内存，
    因为每次只下载小部分数据。
    '''

    if not os.path.exists(savePath):
        os.makedirs(savePath)

    # if 'win' in sys.platform:
    #     saveFilePath = savePath + '\\' + fileName
    # else:
    #     saveFilePath = savePath + '/' + fileName

    response = requests.get(fileUrl, stream=True, timeout=downloadTimeout)
    # response = requests.get(fileUrl, stream=True)
    saveFile = open(savePath + fileName, mode)
    for chunk in response.iter_content(chunk_size=1024):
        if chunk:
            saveFile.write(chunk)

    saveFile.close()

    # try:
    #     pass
    # except NameError:
    #     pass
    # except TypeError:
    #     pass
    # except Exception:
    #     pass
    # else:
    #     pass
    # finally:
    #     pass


def main():
    # 请求不了
    # poolManager = urllib3.PoolManager()
    # httpResponse = poolManager.request(
    #     "GET",
    #     "https://api.500px.com/v1/photos?image_size[]=2&image_size[]=3&image_size[]=4&image_size[]=1080&image_size[]=2048&image_size[]=440&original_size[gte]=1080&consumer_key=rvPErS5KPfLtcXQBDHOLwWVXvBi94LA1JLbdJghN&exclude=Nude,People,Fashion&feature=popular&only=All&page=1&rpp=30",
    #     headers=headers)
    # print(httpResponse.status)
    # print(httpResponse.data)

    print('main() start')
    # imageCount = 3780
    imagesFile = open(imagesFilePath, "a")
    pageFile = open(pageFilePath, "r+")
    imageCountFile = open(imageCountFilePath, "r+")

    startPageInt = int(pageFile.readline())
    print("startPageInt = %d" % (startPageInt))

    imageCountInt = int(imageCountFile.readline())
    print("imageCountInt = %d" % (imageCountInt))
    imageCount = imageCountInt

    # for page in range(127, 1001):
    for page in range(startPageInt, 4002):
        headers = {
            "Origin": "http://www.500px.tv/",
            "Referer": "http://www.500px.tv/",
            "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.186 Safari/537.36",
            'only': 'All',
            'page': str(page),
            'rpp': '30'
        }
        '''
        http://www.500px.tv/#/dashboard/popular/0/1
        怎样得到下面这个地址是个关键的步骤：
    
        '''
        try:
            response = requests.get(
                "https://api.500px.com/v1/photos?image_size[]=2&image_size[]=3&image_size[]=4&image_size[]=1080&image_size[]=2048&image_size[]=440&original_size[gte]=1080&consumer_key=rvPErS5KPfLtcXQBDHOLwWVXvBi94LA1JLbdJghN&exclude=Nude,People,Fashion&feature=popular&only=All&page=1&rpp=30",
                params=headers,
                timeout=requestTimeout
            )
        except Exception:
            print('第 %d 页请求出现异常,60秒后再次请求' % (page))
            time.sleep(60)
            response = requests.get(
                "https://api.500px.com/v1/photos?image_size[]=2&image_size[]=3&image_size[]=4&image_size[]=1080&image_size[]=2048&image_size[]=440&original_size[gte]=1080&consumer_key=rvPErS5KPfLtcXQBDHOLwWVXvBi94LA1JLbdJghN&exclude=Nude,People,Fashion&feature=popular&only=All&page=1&rpp=30",
                params=headers,
                timeout=requestTimeout
            )

        # response = requests.post(
        #     # "http://www.500px.tv/#/dashboard/popular/0/1",
        #     "https://api.500px.com/v1/photos?image_size[]=2&image_size[]=3&image_size[]=4&image_size[]=1080&image_size[]=2048&image_size[]=440&original_size[gte]=1080&consumer_key=rvPErS5KPfLtcXQBDHOLwWVXvBi94LA1JLbdJghN&exclude=Nude,People,Fashion&feature=popular&only=All&page=1&rpp=30",
        #     params=headers,
        #     data=form_data
        # )

        print('第 %d 页请求成功' % (page))

        strResult = response.content.decode("utf-8")
        jsonResult = json.loads(strResult)
        getList = jsonResult['photos']
        # print(getList)
        for temp in getList:
            # temp is 'dict'
            # 大图的地址
            imageUrl = temp['images'][-1]['https_url']
            imageName = imageUrl[31:40] + '.jpg'
            print('-' * 50)
            global savingImageFilePath
            savingImageFilePath = saveImagesFilePath + imageName
            print(savingImageFilePath)
            if not os.path.exists(savingImageFilePath):
                print('正在保存 %s 这个文件' % (imageName))
                saveFile(imageUrl, saveImagesFilePath, imageName, 'wb')

                imagesFile.write(imageUrl + '\n')
                imagesFile.flush()
                print(imageUrl)

                imageCount += 1;
                imageCountFile.seek(0)
                imageCountFile.write(str(imageCount))
                imageCountFile.flush()
                print('imageCount = %d' % (imageCount))

        pageFile.seek(0)
        pageFile.write(str(page))
        pageFile.flush()
        print('page = %d' % (page))
        page += 1
        time.sleep(10)

    imagesFile.close()
    pageFile.close()
    imageCountFile.close()

    print('main() end')
    # soup = BeautifulSoup(strResult, "html.parser")


def downloadFile():
    print("正在下载图片......")
    imagesFile = open('D:\\picture\\500px\\images_url.txt', 'r')
    while 1:
        getLineContent = imagesFile.readline()
        if len(getLineContent) == 0:
            print("downloadFile over")
            break

        # 在windows中这句很关键
        getLineContent = getLineContent[:-1:]

        if os.path.exists('D:\\picture\\500px\\images\\' + getLineContent[31:40] + '.jpg'):
            continue

        print(getLineContent)
        saveFile(getLineContent, 'D:\\picture\\500px\\images\\', getLineContent[31:40] + '.jpg', 'wb')

    imagesFile.close()
    print("图片下载完成")


if __name__ == '__main__':

    basePath = '/mnt/d/picture/500px/'
    basePath = '/root/Pictures/500px/'
    imagesFilePath = basePath + 'images_url.txt'
    pageFilePath = basePath + 'page.txt'
    imageCountFilePath = basePath + 'imageCount.txt'
    saveImagesFilePath = basePath + 'images/'
    requestTimeout = 60
    downloadTimeout = 30
    savingImageFilePath = saveImagesFilePath + '1.jpg'

    while 1:
        pageFile = open(pageFilePath, "r")
        pageInt = int(pageFile.readline())
        print('while page = %d' % (pageInt))
        if pageInt > 4000:
            print('while break')
            pageFile.close()
            break

        pageFile.close()

        try:
            main()
            pass
        except Exception:
            print('Exception Occur')
            if os.path.exists(savingImageFilePath):
                os.remove(savingImageFilePath)
                print('%s 文件已被删除'%(savingImageFilePath))

            time.sleep(60)

    # downloadFile()

    # downloadFile = threading.Thread(target=downloadFile)
    # downloadFile.start()
    # downloadFile.join()
    # print("main downloadFile over")

    # getLineContent='https://drscdn.500px.org/photo/248842083/m%3D2048_a%3D2/v2?client_application_id=7168&webp=true&sig=71578a26f281d05c36e81a8f4903bc46d9342dc3c7e4159c49bbd4cebfb08632'
    # saveFile(
    #     getLineContent,
    #     'D:\\picture\\500px\\images',
    #     getLineContent[31:40] + '.jpg',
    #     'wb'
    # )

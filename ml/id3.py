#!/usr/bin/env python

import fileinput
import math

def dataEntropy(dataList, i):
    attrsMap = {}
    total = 0
    for data in dataList:
        value = attrsMap.get(data[i], 0)
        attrsMap[data[i]] = value + 1
        total += 1
    entropy = 0
    for value in attrsMap.itervalues():
        p = 1.0 * value / total
        entropy +=  -p*math.log(p, 2)
    #print entropy
    #print dataList
    return entropy


def getSubDataListMap(dataList, i):
    subDataListMap = {}
    for data in dataList:
        subDataList = subDataListMap.get(data[i], [])
        subDataList.append(data)
        subDataListMap[data[i]] = subDataList
    return subDataListMap

def getGain(dataList, i):

    entropyNow = dataEntropy(dataList, -1)
    subDataListMap = getSubDataListMap(dataList, i)
    subEntropyTotal = 0
    for subDataList in subDataListMap.itervalues():
        subListEntropy = dataEntropy(subDataList, -1)
        subEntropyTotal  +=  (len(subDataList) * 1.0 /  len(dataList)) * subListEntropy

    return entropyNow - subEntropyTotal

def selectAttribute(dataList,attributes):
    maxGain  = 0.0
    maxAttr = ''
    for attr in attributes:
        gain = getGain(dataList, attr)
        if gain > maxGain:
            maxGain = gain
            maxAttr = attr
    return maxAttr

def id3(dataList,attributes, path):
    currentEntropy = dataEntropy(dataList, -1)
    if currentEntropy == 0:
        subDataListMap = getSubDataListMap(dataList, -1)
        for key, subDataList in subDataListMap.iteritems():
            print path
            print key.strip()
            for data in subDataList:
                print data[0],
            print
        pass
    else:
        if len(attributes) == 0:
            subDataListMap = getSubDataListMap(dataList, -1)
            maxKey = 0
            maxSize = -1
            for key, subDataList in subDataListMap.iteritems():
                if len(subDataList) > maxSize:
                    maxSize = len(subDataList)
                    maxKey = key
            print path
            print maxKey.strip()
            for data in dataList:
                print data[0],
            print
        else:
            maxAttr = selectAttribute(dataList, attributes)
            attributes.remove(maxAttr)
            subDataListMap = getSubDataListMap(dataList, maxAttr)
            for attrValue, subDataList in subDataListMap.iteritems():
                path.append((maxAttr, attrValue))
                id3(subDataList, attributes, path)
                path.pop()

dataList = []
for line in fileinput.input():
    data = line.split(' ')
    dataList.append(data)

#print dataEntropy(dataList, -1)

#print getGain(dataList, 1)
#print getGain(dataList, 2)
#print getGain(dataList, 3)
#print getGain(dataList, 4)
#print selectAttribute(dataList, [1,2, 3, 4])

id3(dataList, [1,2, 3, 4], [])

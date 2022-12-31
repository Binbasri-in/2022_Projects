import json, unittest, datetime

with open("D:\python\Coursera\Forage\data-1.json","r") as f:
    jsonData1 = json.load(f)
with open("D:\python\Coursera\Forage\data-2.json","r") as f:
    jsonData2 = json.load(f)
with open("D:\python\Coursera\Forage\data-result.json","r") as f:
    jsonExpectedResult = json.load(f)


def convertFromFormat1 (jsonObject):

    # IMPLEMENT: Conversion From Type 1
    result = {}
    result['deviceID'] = jsonObject['deviceID']
    result['deviceType'] = jsonObject['deviceType']
    result['timestamp'] = jsonObject['timestamp']
    location = jsonObject['location'].split('/')

    result['location'] = {
    'country':location[0],
    'city':location[1],
    'area':location[2],
    'factory':location[3],
    'section':location[4]
    }

    result['data'] = {
    'status':jsonObject['operationStatus'],
    'temperature':jsonObject['temp']
    }

    return result


def convertFromFormat2 (jsonObject):

    # IMPLEMENT: Conversion From Type 1
    result = {}
    result['deviceID'] = jsonObject['device']['id']
    result['deviceType'] = jsonObject['device']['type']
    time = jsonObject['timestamp']
    date = datetime.datetime.strptime(time, '%Y-%m-%dT%H:%M:%S.%fZ')
    timestamp = str((date - datetime.datetime(1970, 1, 1)).total_seconds()*1000)
    result['timestamp'] = timestamp
    result['location'] = {
    'country':jsonObject['country'],
    'city':jsonObject['city'],
    'area':jsonObject['area'],
    'factory':jsonObject['factory'],
    'section':jsonObject['section']
    }

    result['data'] = jsonObject['data']

    return result


def main (jsonObject):

    result = {}

    if (jsonObject.get('device') == None):
        result = convertFromFormat1(jsonObject)
    else:
        result = convertFromFormat2(jsonObject)

    return result


class TestSolution(unittest.TestCase):

    def test_sanity(self):

        result = json.loads(json.dumps(jsonExpectedResult))
        self.assertEqual(
            result,
            jsonExpectedResult
        )

    def test_dataType1(self):

        result = main (jsonData1)
        self.assertEqual(
            result,
            jsonExpectedResult,
            'Converting from Type 1 failed'
        )

    def test_dataType2(self):

        result = main (jsonData2)
        self.assertEqual(
            result,
            jsonExpectedResult,
            'Converting from Type 2 failed'
        )

if __name__ == '__main__':
    unittest.main()

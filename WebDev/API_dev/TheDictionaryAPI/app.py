# Description: This is an API that recives a word on the define endpoint and returns the json data 
# from the scrapped website.
from flask import Flask, jsonify, request
# import the scraper
from wrpy import WordReference
# create an instance of the scraper for english to arabic
wr = WordReference('en', 'ar')

# create a Flask app
app = Flask(__name__)

# create a route for the app
@app.route('/try', methods=['GET'])
def define():
    # get the word from the request
    word = request.args.get('word')
    # send the word to the scraper
    data = scraper(word)
    # if the word is not found, return a 404 error
    if data == None:
        return jsonify({'error': 'Word not found', 'status': 404, 'message': 'Not Found'}), 404

    # add the status and message to the data
    data['status'] = 200
    data['message'] = 'Found'
    
    # return the data and keep the format to support arabic
    return jsonify(data), 200

# create a function to scrape the data
def scraper(word):
    # check the word is as the format
    word = check_word(word)
    # if the word is not valid, return None
    if word == None:
        return None
    # get the data from the scraper
    data = wr.translate(word)
    # if the word is not found, return None
    if data == None:
        return None
    # create a dictionary to store the data
    data_dict = {}
    # add the word to the dictionary
    data_dict['word'] = word
    # define the lists
    data_dict['translation'] = []
    data_dict['meanings'] = []
    # add the audio url
    data_dict['audio'] = check_audio(f'https://d1qx7pbj0dvboc.cloudfront.net/{word}.mp3')
    # for each meaning in the data////////////////////////////////////////////////////////////////////////////////////////////////////////////
    count = 0
    for meaning in data['translations'][0]['entries']:
        # take only the first 3 meanings
        if count == 3:
            break
        # add the translation
        data_dict['translation'].append(meaning['to_word'][0]['meaning'])
        # add the part_of_speech, definition and example
        data_dict['meanings'].append({'part_of_speech': map_pos(meaning['from_word']['grammar']),
                                        'meaning': {'definition': meaning['context'],
                                        'example': meaning['from_example'],
                                        'example_ar': meaning['to_example']}})
        count += 1
    # return the data
    display(data_dict)
    return data_dict


# create a function to check the word is valid
def check_word(word):
    return word

def display(data):
    print(data['word'])
    print(data['translation'])
    print(data['audio'])
    for meaning in data['meanings']:
        print(meaning['part of speech'])
        print(meaning['meaning']['definition'])
        print(meaning['meaning']['example'])
        print(meaning['meaning']['example_ar'])

def map_pos(pos):
    # map the part of speech to the correct format
    if pos == 'n':
        return 'noun'
    elif pos.startswith('v'):
        return 'verb'
    elif pos == 'adj':
        return 'adjective'
    elif pos == 'adv':
        return 'adverb'
    elif pos == 'prep':
        return 'preposition'
    elif pos == 'pron':
        return 'pronoun'
    elif pos == 'conj':
        return 'conjunction'
    elif pos == 'interj':
        return 'interjection'
    else:
        return 'other'

# function to check the url of the audio is valid
def check_audio(url):
    # if the url is valid, return the url
    if url != 'https://d1qx7pbj0dvboc.cloudfront.net/.mp3':
        return url
    # else return None
    return None
# run the app
if __name__ == '__main__':
    app.run()



# example of a response in success
# {
#     "word": "hello",
#     "translation": ["مرحبا", "اهلا"],
#     "audio": "https://lex-audio.useremarkable.com/mp3/hello_us_1_rr.mp3"
#    
#     "meanings": [{
#         "part_of_speech": "interjection",
#         "meaning": {
#             "definition": "used as a greeting or to begin a telephone conversation.",
#             "example": "hello, is that Mr Smith?"
#         }}, {
#         "part of_speech": "noun",
#         "meaning": {
#             "definition": "an utterance of ‘hello’; a greeting.",
#             "example": "she was getting polite nods and hellos from people she passed in the street"
#         }}
#     ],
#     "status": 200,
#     "message": "Found"
# }

# example of a response in failure
# {
#     "error": "Word not found",
#     "status": 404,
#     "message": "Not Found"
# }
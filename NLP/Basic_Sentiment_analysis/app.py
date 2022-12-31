# This is a web application that takes the ID of a mobile application on App Store
# and apply sentiment analysis on the reviewsof the application and return the result
# to the user. The result is a pie chart that shows the percentage of positive, negative
# and neutral reviews.

# Import libraries
from flask import Flask, render_template, request
from app_store_scraper import AppStore
import pandas as pd
import numpy as np
# Importing the libraries
import nltk
nltk.download('vader_lexicon')
from nltk.sentiment.vader import SentimentIntensityAnalyzer

# open the database using sqlite3
import sqlite3



# Import the function that does the sentiment analysis
def basic_sentiment_analysis(app_name, app_id):
    conn = sqlite3.connect('app_ratings.db')
    c = conn.cursor()
    # check if the app exists in the database
    c.execute('''SELECT * FROM results WHERE app_id = ?''', (app_id, ))
    data = c.fetchall()
    if len(data) != 0:
        # if the app exists in the database, return the result
        negative = data[0][2]
        positive = data[0][3]
        neutral = data[0][4]
        return (negative, positive, neutral)
    the_app = AppStore(country='in', app_name=app_name, app_id = app_id)
    # check if the data was scraped successfully
    #if the_app.review(how_many=1) == None:
    #    return 'The app was not found. Please try again.'
    # scrape the data
    the_app.review(how_many=1000)
    df = pd.DataFrame(np.array(the_app.reviews),columns=['review'])
    df = df.join(pd.DataFrame(df.pop('review').tolist()))

    # create a list of the latest 500 reviews and svae it in a csv file
    latest500 = df.sort_values(by=['date'],ascending=False).head(500)

    sid = SentimentIntensityAnalyzer()

    # Creating a new column 'Compound' which contains the polarity scores
    latest500['Compound'] = latest500['review'].apply(lambda x: sid.polarity_scores(x)['compound'])

    # Creating a new column 'Sentiment' which classifies the polarity scores
    latest500['Sentiment'] = latest500['Compound'].apply(lambda x: 'Positive' if x >=0 else 'Negative')

    # Printing the first 10 rows of the data
    print(latest500.head(10))

    # Printing the percentage of positive and negative reviews
    results = latest500['Sentiment'].value_counts(normalize=True) * 100
    
    
    # get the results of Negative, Positive and Neutral reviews
    negative = results[0]
    print(negative)
    positive = results[1]
    print(positive)
    neutral = 100 - negative - positive
    print(neutral)

    # save this result in the database # TODO
    # insert the result in the table
    c.execute('''INSERT INTO results VALUES(?,?,?,?,?)''', (app_name, app_id, negative, positive, neutral))
    # commit the changes
    conn.commit()

    # return the result to the user
    return (negative, positive, neutral)


# Create the Flask app
app = Flask(__name__)

# Create the route
@app.route('/', methods=['GET', 'POST'])
def index():
    if request.method == 'POST':
        # Get the app name and app id from the form
        app_name = request.form['app_name']
        app_id = request.form['app_id']
        # Call the function that does the sentiment analysis
        try:
            result = basic_sentiment_analysis(app_name, app_id)
        except:
            return render_template('result.html', result='The app was not found. Please try again.')
        # convert the result to a list
        result = list(result)
        # the final statement
        result = [round(i, 2) for i in result]
        string = 'The percentage of negative reviews is ' + str(result[0]) + '%, the percentage of positive reviews is ' + str(result[1]) + '% and the percentage of neutral reviews is ' + str(result[2]) + '%.'
        # Return the result to the user
        return render_template('result.html', result=string)
    else:
        return render_template('index.html')

# Run the app
if __name__ == '__main__':
    app.run(debug=True)
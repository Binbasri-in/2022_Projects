# a web application for Bondora loans Risk Analysis

from flask import Flask, render_template, request
import requests
import pickle
import numpy as np
import pandas as pd

app = Flask(__name__)
# import the model Default_svc.sav and Numerical_target_GBR.sav
default_model = pickle.load(open('pipeline_class.pkl', 'rb'))
num_target_model= pickle.load(open('pipeline_reg.pkl', 'rb'))

@app.route('/',methods=['GET', 'POST'])
def index():
    return render_template('index.html', prediction_text='', prediction_text_num='')


@app.route("/predict", methods=['POST', 'GET'])
def predict():
    if request.method == 'POST':
        features = {'BidsPortfolioManager': float(request.form['BidsPortfolioManager']), 'BidsApi': float(request.form['BidsApi']),
                    'BidsManual': float(request.form['BidsManual']), 'Age': int(request.form['Age']), 
                    'AppliedAmount': float(request.form['AppliedAmount']), 'Interest': float(request.form['Interest']),
                    'MonthlyPayment': float(request.form['MonthlyPayment']), 'IncomeTotal': float(request.form['IncomeTotal']),
                    'ExistingLiabilities': int(request.form['ExistingLiabilities']), 'LiabilitiesTotal': float(request.form['LiabilitiesTotal']),
                    'RefinanceLiabilities': 0, 'DebtToIncome': float(request.form['DebtToIncome']),
                    'FreeCash': float(request.form['FreeCash']), 'PrincipalPaymentsMade': float(request.form['PrincipalPaymentsMade']),
                    'InterestAndPenaltyPaymentsMade': float(request.form['InterestAndPenaltyPaymentsMade']), 'PrincipalBalance': float(request.form['PrincipalBalance']),
                    'InterestAndPenaltyBalance': float(request.form['InterestAndPenaltyBalance']), 'PreviousRepaymentsBeforeLoan': float(request.form['PreviousRepaymentsBeforeLoan']),
                    'NewCreditCustomer_New_credit_Customer': 0,
                    'VerificationType_Income unverified, cross-referenced by phone': 0, 'VerificationType_Income verified': 0,
                    'VerificationType_IncomeUnverified': 0, 'VerificationType_NotSet': 0, 'LanguageCode_Estonian': 0,
                    'LanguageCode_Finnish': 0, 'LanguageCode_German': 0, 'LanguageCode_Other': 0, 'LanguageCode_Russian': 0,
                    'LanguageCode_Slovakian': 0, 'LanguageCode_Spanish': 0, 'Gender_Male': 0, 'Gender_Undefined': 0,
                    'Country_Finland': 0, 'Country_Slovakia': 0, 'Country_Spain': 0, 'UseOfLoan_Acquisition of real estate': 0,
                    'UseOfLoan_Acquisition of stocks': 0, 'UseOfLoan_Business': 0, 'UseOfLoan_Construction finance': 0,
                    'UseOfLoan_Education': 0, 'UseOfLoan_Health': 0, 'UseOfLoan_Home improvement': 0,
                    'UseOfLoan_Loan consolidation': 0, 'UseOfLoan_Not set': 0, 'UseOfLoan_Other': 0, 'UseOfLoan_Other business': 0,
                    'UseOfLoan_Purchase of machinery equipment': 0, 'UseOfLoan_Real estate': 0, 'UseOfLoan_Travel': 0,
                    'UseOfLoan_Vehicle': 0, 'UseOfLoan_Working capital financing': 0, 'Education_Higher education': 0,
                    'Education_Not_present': 0, 'Education_Primary education': 0, 'Education_Secondary education': 0,
                    'Education_Vocational education': 0, 'MaritalStatus_Divorced': 0, 'MaritalStatus_Married': 0,
                    'MaritalStatus_Not_specified': 0, 'MaritalStatus_Single': 0, 'MaritalStatus_Widow': 0, 'EmploymentStatus_Fully employed': 0,
                    'EmploymentStatus_Not_specified': 0, 'EmploymentStatus_Partially employed': 0, 'EmploymentStatus_Retiree': 0,
                    'EmploymentStatus_Self-employed': 0, 'EmploymentDurationCurrentEmployer_Other': 0,
                    'EmploymentDurationCurrentEmployer_Retiree': 0, 'EmploymentDurationCurrentEmployer_TrialPeriod': 0,
                    'EmploymentDurationCurrentEmployer_UpTo1Year': 0, 'EmploymentDurationCurrentEmployer_UpTo2Years': 0,
                    'EmploymentDurationCurrentEmployer_UpTo3Years': 0, 'EmploymentDurationCurrentEmployer_UpTo4Years': 0,
                    'EmploymentDurationCurrentEmployer_UpTo5Years': 0, 'OccupationArea_Agriculture, forestry and fishing': 0,
                    'OccupationArea_Art and entertainment': 0, 'OccupationArea_Civil service & military': 0,
                    'OccupationArea_Construction': 0, 'OccupationArea_Education': 0, 'OccupationArea_Energy': 0,
                    'OccupationArea_Finance and insurance': 0, 'OccupationArea_Healthcare and social help': 0,
                    'OccupationArea_Hospitality and catering': 0, 'OccupationArea_Info and telecom': 0, 'OccupationArea_Mining': 0,
                    'OccupationArea_Not_specified': 0, 'OccupationArea_Other': 0, 'OccupationArea_Processing': 0,
                    'OccupationArea_Real-estate': 0, 'OccupationArea_Research': 0, 'OccupationArea_Retail and wholesale': 0,
                    'OccupationArea_Transport and warehousing': 0, 'OccupationArea_Utilities': 0, 'HomeOwnershipType_Homeless': 0,
                    'HomeOwnershipType_Joint ownership': 0, 'HomeOwnershipType_Joint tenant': 0, 'HomeOwnershipType_Living with parents': 0,
                    'HomeOwnershipType_Mortgage': 0, 'HomeOwnershipType_Not_specified': 0, 'HomeOwnershipType_Other': 0,
                    'HomeOwnershipType_Owner': 0, 'HomeOwnershipType_Owner with encumbrance': 0,
                    'HomeOwnershipType_Tenant, pre-furnished property': 0, 'HomeOwnershipType_Tenant, unfurnished property': 0,
                    'Rating_AA': 0, 'Rating_B': 0, 'Rating_C': 0, 'Rating_D': 0, 'Rating_E': 0, 'Rating_F': 0, 'Rating_HR': 0,
                    'Rating_Not_specified': 0, 'Restructured_Yes': 0, 'CreditScoreEsMicroL_M1': 0, 'CreditScoreEsMicroL_M10': 0,
                    'CreditScoreEsMicroL_M2': 0, 'CreditScoreEsMicroL_M3': 0, 'CreditScoreEsMicroL_M4': 0,
                    'CreditScoreEsMicroL_M5': 0, 'CreditScoreEsMicroL_M6': 0, 'CreditScoreEsMicroL_M7': 0,
                    'CreditScoreEsMicroL_M8': 0, 'CreditScoreEsMicroL_M9': 0}

        
        # add the options selected by the user to the features dictionary
        temp = request.form['NewCreditCustomer']
        if temp == 'New_credit_Customer':
            features['NewCreditCustomer_New_credit_Customer'] = 1
        
        temp = request.form['VerificationType']
        if temp == 'Income unverified, cross-referenced by phone':
            features['VerificationType_Income unverified, cross-referenced by phone'] = 1
        elif temp == 'Income verified':
            features['VerificationType_Income verified'] = 1
        elif temp == 'IncomeUnverified':
            features['VerificationType_IncomeUnverified'] = 1
        elif temp == 'NotSet':
            features['VerificationType_NotSet'] = 1

        temp = request.form['LanguageCode']
        if temp == 'Estonian':
            features['LanguageCode_Estonian'] = 1
        elif temp == 'Finnish':
            features['LanguageCode_Finnish'] = 1
        elif temp == 'German':
            features['LanguageCode_German'] = 1
        elif temp == 'Russian':
            features['LanguageCode_Russian'] = 1
        elif temp == 'Slovakian':
            features['LanguageCode_Slovakian'] = 1
        elif temp == 'Spanish':
            features['LanguageCode_Spanish'] = 1
        elif temp == 'Swedish':
            features['LanguageCode_Swedish'] = 1

        temp = request.form['Gender']
        if temp == 'Male':
            features['Gender_Male'] = 1
        elif temp == 'Undefined':
            features['Gender_Undefined'] = 1

        temp = request.form['Country']
        # if temp == 'Estonia':
        #     features['Country_Estonia'] = 1
        if temp == 'Finland':
            features['Country_Finland'] = 1
        elif temp == 'Slovakia':
            features['Country_Slovakia'] = 1
        elif temp == 'Spain':
            features['Country_Spain'] = 1

        temp = request.form['UseOfLoan']
        if temp == 'Home improvement':
            features['UseOfLoan_Home improvement'] = 1
        elif temp == 'Other':
            features['UseOfLoan_Other'] = 1
        elif temp == 'Loan consolidation':
            features['UseOfLoan_Loan consolidation'] = 1
        elif temp == 'Not set':
            features['UseOfLoan_Not set'] = 1
        elif temp == 'Vehicle':
            features['UseOfLoan_Vehicle'] = 1
        elif temp == 'Education':
            features['UseOfLoan_Education'] = 1
        elif temp == 'Business':
            features['UseOfLoan_Business'] = 1
        elif temp == 'Travel':
            features['UseOfLoan_Travel'] = 1
        elif temp == 'Health':
            features['UseOfLoan_Health'] = 1
        elif temp == 'Real estate':
            features['UseOfLoan_Real estate'] = 1
        elif temp == 'Purchase of machinery equipment':
            features['UseOfLoan_Purchase of machinery equipment'] = 1
        elif temp == 'Other business':
            features['UseOfLoan_Other business'] = 1
        elif temp == 'Accounts receivable financing':
            features['UseOfLoan_Accounts receivable financing'] = 1
        elif temp == 'Working capital financing':
            features['UseOfLoan_Working capital financing'] = 1
        elif temp == 'Acquisition of stocks':
            features['UseOfLoan_Acquisition of stocks'] = 1
        elif temp == 'Acquisition of real estate':
            features['UseOfLoan_Acquisition of real estate'] = 1
        elif temp == 'Construction finance':
            features['UseOfLoan_Construction finance'] = 1
        
        temp = request.form['Education']
        if temp == 'Secondary education':
            features['Education_Secondary education'] = 1
        elif temp == 'Higher education':
            features['Education_Higher education'] = 1
        elif temp == 'Vocational education':
            features['Education_Vocational education'] = 1
        elif temp == 'Basic education':
            features['Education_Basic education'] = 1
        elif temp == 'Not_present':
            features['Education_Not_present'] = 1
        elif temp == 'Primary education':
            features['Education_Primary education'] = 1

        temp = request.form['MaritalStatus']
        if temp == 'Single':
            features['MaritalStatus_Single'] = 1
        elif temp == 'Married':
            features['MaritalStatus_Married'] = 1
        elif temp == 'Not_specified':
            features['MaritalStatus_Not_specified'] = 1
        elif temp == 'Divorced':
            features['MaritalStatus_Divorced'] = 1
        elif temp == 'Widow':
            features['MaritalStatus_Widow'] = 1
        elif temp == 'Cohabitant':
            features['MaritalStatus_Cohabitant'] = 1

        temp = request.form['EmploymentStatus']
        if temp == 'Fully employed':
            features['EmploymentStatus_Fully employed'] = 1
        elif temp == 'Entrepreneur':
            features['EmploymentStatus_Entrepreneur'] = 1
        elif temp == 'Self-employed':
            features['EmploymentStatus_Self-employed'] = 1
        elif temp == 'Partially employed':
            features['EmploymentStatus_Partially employed'] = 1
        elif temp == 'Not_specified':
            features['EmploymentStatus_Not_specified'] = 1

        temp = request.form['EmploymentDurationCurrentEmployer']
        # if temp == 'MoreThan5Years':
        #     features['EmploymentDurationCurrentEmployer_MoreThan5Years'] = 1
        if temp == 'UpTo1Year':
            features['EmploymentDurationCurrentEmployer_UpTo1Year'] = 1
        elif temp == 'UpTo5Years':
            features['EmploymentDurationCurrentEmployer_UpTo5Years'] = 1
        elif temp == 'UpTo2Years':
            features['EmploymentDurationCurrentEmployer_UpTo2Years'] = 1
        elif temp == 'UpTo3Years':
            features['EmploymentDurationCurrentEmployer_UpTo3Years'] = 1
        elif temp == 'UpTo4Years':
            features['EmploymentDurationCurrentEmployer_UpTo4Years'] = 1
        elif temp == 'Retiree':
            features['EmploymentDurationCurrentEmployer_Retiree'] = 1
        elif temp == 'Other':
            features['EmploymentDurationCurrentEmployer_Other'] = 1
        elif temp == 'TrialPeriod':
            features['EmploymentDurationCurrentEmployer_TrialPeriod'] = 1

        temp = request.form['OccupationArea']
        if temp == 'Not_specified':
            features['OccupationArea_Not_specified'] = 1
        elif temp == 'Other':
            features['OccupationArea_Other'] = 1
        elif temp == 'Agriculture, forestry and fishing':
            features['OccupationArea_Agriculture, forestry and fishing'] = 1
        elif temp == 'Art and entertainment':
            features['OccupationArea_Art and entertainment'] = 1
        elif temp == 'Civil service & military':
            features['OccupationArea_Civil service & military'] = 1
        elif temp == 'Construction':
            features['OccupationArea_Construction'] = 1
        elif temp == 'Education':
            features['OccupationArea_Education'] = 1
        elif temp == 'Energy':
            features['OccupationArea_Energy'] = 1
        elif temp == 'Finance and insurance':
            features['OccupationArea_Finance and insurance'] = 1
        elif temp == 'Healthcare and social help':
            features['OccupationArea_Healthcare and social help'] = 1
        elif temp == 'Hospitality and catering':
            features['OccupationArea_Hospitality and catering'] = 1
        elif temp == 'Info and telecom':
            features['OccupationArea_Info and telecom'] = 1
        elif temp == 'Mining':
            features['OccupationArea_Mining'] = 1
        elif temp == 'Processing':
            features['OccupationArea_Processing'] = 1
        elif temp == 'Research':
            features['OccupationArea_Research'] = 1
        elif temp == 'Retail and wholesale':
            features['OccupationArea_Retail and wholesale'] = 1
        elif temp == 'Transport and warehousing':
            features['OccupationArea_Transport and warehousing'] = 1
        elif temp == 'Utilities':
            features['OccupationArea_Utilities'] = 1

        temp = request.form['HomeOwnershipType']
        if temp == 'Homeless':
            features['HomeOwnershipType_Homeless'] = 1
        elif temp == 'Mortgage':
            features['HomeOwnershipType_Mortgage'] = 1
        elif temp == 'Joint ownership':
            features['HomeOwnershipType_Joint ownership'] = 1
        elif temp == 'Joint tenant':
            features['HomeOwnershipType_Joint tenant'] = 1
        elif temp == 'Living with parents':
            features['HomeOwnershipType_Living with parents'] = 1
        elif temp == 'Not_specified':
            features['HomeOwnershipType_Not_specified'] = 1
        elif temp == 'Owner':
            features['HomeOwnershipType_Owner'] = 1
        elif temp == 'Other':
            features['HomeOwnershipType_Other'] = 1
        elif temp == 'Owner with encumbrance':
            features['HomeOwnershipType_Owner with encumbrance'] = 1
        elif temp == 'Tenant, pre-furnished property':
            features['HomeOwnershipType_Tenant, pre-furnished property'] = 1
        elif temp == 'Tenant, unfurnished property':
            features['HomeOwnershipType_Tenant, unfurnished property'] = 1

        temp = request.form['Rating']
        if temp == 'AA':
            features['Rating_AA'] = 1
        elif temp == 'B':
            features['Rating_B'] = 1
        elif temp == 'C':
            features['Rating_C'] = 1
        elif temp == 'D':
            features['Rating_D'] = 1
        elif temp == 'E':
            features['Rating_E'] = 1
        elif temp == 'F':
            features['Rating_F'] = 1
        elif temp == 'HR':
            features['Rating_HR'] = 1
        elif temp == 'Not_specified':
            features['Rating_Not_specified'] = 1

        temp = request.form['Restructured']
        if temp == 'Yes':
            features['Restructured_Yes'] = 1
        
        temp = request.form['CreditScoreEsMicroL']
        if temp == 'M1':
            features['CreditScoreEsMicroL_M1'] = 1
        elif temp == 'M2':
            features['CreditScoreEsMicroL_M2'] = 1
        elif temp == 'M3':
            features['CreditScoreEsMicroL_M3'] = 1
        elif temp == 'M4':
            features['CreditScoreEsMicroL_M4'] = 1
        elif temp == 'M5':
            features['CreditScoreEsMicroL_M5'] = 1
        elif temp == 'M6':
            features['CreditScoreEsMicroL_M6'] = 1
        elif temp == 'M7':
            features['CreditScoreEsMicroL_M7'] = 1
        elif temp == 'M8':
            features['CreditScoreEsMicroL_M8'] = 1
        elif temp == 'M9':
            features['CreditScoreEsMicroL_M9'] = 1
        elif temp == 'M10':
            features['CreditScoreEsMicroL_M10'] = 1

        # TODO: more features need to be preprocessed
        # calculate 'Ava_Inc' and 'InterestAmount' LoanTenure
        
        arr = np.array(list(features.values())).reshape(1, -1)
        print(arr)
        output = default_model.predict(arr)

        output_num = num_target_model.predict(arr)
        

        return render_template('result.html', prediction_text='The probability of Default is {}'.format(output), prediction_text_num='The Numerical Target is {}'.format(output_num))
    else:
        return render_template('result.html', prediction_text='', prediction_text_num='')

if __name__ == "__main__":
    app.run(debug=True)
#!/opt/anaconda2/bin/python
# -*- coding: utf-8 -*-

"""
################################################################################
#
#  Copyright (c) 2016 Wojciech Migda
#  All rights reserved
#  Distributed under the terms of the MIT license
#
################################################################################
#
#  Filename: avnet_estimator.py
#
#  Decription:
#      AVNET estimator
#
#  Authors:
#       Wojciech Migda
#
################################################################################
#
#  History:
#  --------
#  Date         Who  Ticket     Description
#  ----------   ---  ---------  ------------------------------------------------
#  2016-03-17   wm              Initial version
#
################################################################################
"""

from __future__ import print_function


DEBUG = False

__all__ = []
__version__ = "0.0.1"
__date__ = '2016-03-17'
__updated__ = '2016-03-17'

"""
Index,Name,Type,Description,Values
1,PRODUCT_NUMBER,Integer,Product number,"[1249, 10283064]"
9,CUSTOMER_SEGMENT1,Character,Customer segment with respect to dimension 1,A and B
29,SPECIAL_PART,Character,Special part indicator,"Maybe, No, and Yes"


3,TRANSACTION_DATE,Date,Transaction date,"[2012-07-27, 2015-08-03]"
15,CUSTOMER_FIRST_ORDER_DATE,Date,Date of customer's first order,"[1990-10-19, 2015-04-16]"


Good:
4,PRODUCT_PRICE,Float,"GROSS_SALES / TOTAL_BOXES_SOLD if PRODUCT_SALES_UNIT = N,
  GROSS_SALES / SHIPPING_WEIGHT if PRODUCT_SALES_UNIT = Y",[1.05 to 181.49]
5,GROSS_SALES,Float,Gross sales total (Nb. negative values represent returns),[0 to 5664.82]
11,CUSTOMER_TYPE1,Integer,Customer type,"[1, 3]"
12,CUSTOMER_TYPE2,Character,"Customer type, a subset of CUSTOMER_TYPE1","A, B, and C"
13,CUSTOMER_MANAGED_LEVEL,Character,"Level at which customer is managed, N = National and L = Local",L
14,CUSTOMER_ACCOUNT_TYPE,Character,Customer account type,ST and DM
20,BRAND,Character,Product manufacturer's brand,IN_HOUSE and NOT_IN_HOUSE
22,PRODUCT_SALES_UNIT,Character,"Y = Product sold by weight, N = Product sold by the number of boxes",N and Y
23,SHIPPING_WEIGHT,Float,Total shipping weight (Nb. negative values represent returns),[0 to 11513.25]
24,TOTAL_BOXES_SOLD,Integer,Total boxes sold to the customer (Nb. negative values represent returns),[0 to 301]
25,PRODUCT_COST1,Float,"Product cost estimation 1 (Nb. negative values represent returns).
   This is the cost to the Supplier and is a marker of ""market price"".",[0 to 5664.4]
26,PRODUCT_UNIT_OF_MEASURE,Character,"Product unit of measure - B = Box, LB = Pound, EA = Each","B, EA, and LB"
27,ORDER_SOURCE,Character,How the customer order was placed,A and B
28,PRICE_METHOD,Integer,Method used to price the product,"[1, 5]"


Bad:
                    6,REGION,Integer,Region which sold to the customer,[1]
                    7,WAREHOUSE,Integer,Warehouse which sold to the customer,[1]
                    10,CUSTOMER_SEGMENT2,Integer,Customer segment with respect to dimension 2,[1]
2,CUSTOMER_NUMBER,Integer,Customer number,"[1, 146]"
8,CUSTOMER_ZIP,Integer,Customer zip code,"[1, 98]"
16,PRODUCT_CLASS_ID1,Integer,ID for highest level of product classification,"[1, 12]"
17,PRODUCT_CLASS_ID2,Integer,ID for second highest level of product classification,"[15, 31]"
18,PRODUCT_CLASS_ID3,Integer,ID for second from the lowest level of product classification,"[136, 368]"
19,PRODUCT_CLASS_ID4,Integer,ID for the lowest level of product classification,"[1371, 7772]"
21,PRODUCT_ATTRIBUTE_X,Integer,A certain product attribute,"[5, 999]"

"""


FACTORIZABLE = [
    'CUSTOMER_SEGMENT1', 'CUSTOMER_TYPE2', 'CUSTOMER_MANAGED_LEVEL',
    'CUSTOMER_ACCOUNT_TYPE', 'BRAND', 'PRODUCT_SALES_UNIT',
    'PRODUCT_UNIT_OF_MEASURE', 'ORDER_SOURCE', 'SPECIAL_PART'
    ]

TO_DROP = ['REGION', 'WAREHOUSE', 'CUSTOMER_SEGMENT2']

ATTRIBUTES = ['PRODUCT_CLASS_ID1', 'PRODUCT_CLASS_ID2', 'PRODUCT_CLASS_ID3',
              'PRODUCT_CLASS_ID4',
              'BRAND', # binary
              'PRODUCT_ATTRIBUTE_X',
              'PRODUCT_SALES_UNIT', # binary
              'PRODUCT_UNIT_OF_MEASURE',
              'SPECIAL_PART'
              ]

VARS = ['CUSTOMER_NUMBER', 'TRANSACTION_DATE_1', 'TRANSACTION_DATE_2',
        'TRANSACTION_DATE_3', 'PRODUCT_PRICE', 'GROSS_SALES',
        'CUSTOMER_ZIP', 'CUSTOMER_TYPE1', 'CUSTOMER_TYPE2',
        'CUSTOMER_MANAGED_LEVEL', # binary
        'CUSTOMER_ACCOUNT_TYPE', # binary
        'CUSTOMER_FIRST_ORDER_DATE_1', 'CUSTOMER_FIRST_ORDER_DATE_2',
        'CUSTOMER_FIRST_ORDER_DATE_3', 'SHIPPING_WEIGHT', 'TOTAL_BOXES_SOLD',
        'PRODUCT_COST1',
        'ORDER_SOURCE', #binary
        'PRICE_METHOD']


def OneHot(df, colnames):
    from pandas import get_dummies, concat
    for col in colnames:
        dummies = get_dummies(df[col])
        #ndumcols = dummies.shape[1]
        dummies.rename(columns={p: col + '_' + str(i + 1)  for i, p in enumerate(dummies.columns.values)}, inplace=True)
        df = concat([df, dummies], axis=1)
        pass
    df = df.drop(colnames, axis=1)
    return df


def ParseDates(df, colnames):
    from pandas import concat
    for col in colnames:
        #date, time = df[col].str.split(' ')
        ymd = df[col].str.split(' ').str.get(0)
        y_m_d = ymd.str.split('-', expand=True).astype(int)
        y_m_d.rename(columns={p: col + '_' + str(i + 1) for i, p in enumerate(y_m_d.columns.values)}, inplace=True)
        df = concat([df, y_m_d], axis=1)
        pass
    df = df.drop(colnames, axis=1)
    return df


#from sklearn.base import BaseEstimator, ClassifierMixin


def work(estimator,
         nest,
         njobs,
         nfolds,
         cv_grid,
         clf_kwargs,
         do_hyperopt):

    from numpy.random import seed as random_seed
    random_seed(1)

    from pandas import read_csv

    train = read_csv('../../data/example_data.csv')

    train = train.drop(TO_DROP, axis=1)

    for col in FACTORIZABLE:
        from pandas import factorize
        train[col] = factorize(train[col])[0]
        pass

    train = ParseDates(train, ['TRANSACTION_DATE', 'CUSTOMER_FIRST_ORDER_DATE'])


    grouped = train.groupby(['PRODUCT_NUMBER', 'CUSTOMER_SEGMENT1'])
    samples = []
    for k, v in grouped:
        sample = v.iloc[0][ATTRIBUTES]
        samples.append(sample)
        pass

    from pandas import DataFrame
    train_df = DataFrame.from_records(samples)
    train_y = train_df['SPECIAL_PART'].values
    train_X = train_df.drop(['SPECIAL_PART'], axis=1)

    return


    #train_X['GENDER'][train_X['GENDER'] == 'U'] = float('nan')

    #train_X = OneHot(train_X, NOMINALS)

    # split train and test
    dropped_cols = ['ID', 'TARGET']

    train_y = train['TARGET'].values
    train_X = train.drop(dropped_cols, axis=1)
    test_X = test.drop(dropped_cols, axis=1)



    satan_kwargs = \
    {
        #'objective': 'reg:logistic',
        'objective': 'rank:pairwise',
        'learning_rate': 0.045,
        'min_child_weight': 50,
        'subsample': 0.8,
        'colsample_bytree': 0.7,
        'max_depth': 7,
        'n_estimators': nest,
        'nthread': njobs,
        'seed': 0,
        'cache_opt': 1,
        'missing': float('nan')
        #'scoring': NegQWKappaScorer
    }

    # override kwargs with any changes
    for k, v in clf_kwargs.items():
        satan_kwargs[k] = v
        pass

    from xgb_sklearn import XGBClassifier
    if estimator == 'XGBClassifier':
        clf = XGBClassifier(**satan_kwargs)
        pass
    else:
        clf = globals()[estimator](**satan_kwargs)
        pass

    """ Model crossvalidation """
    if (False
        #or True
        ):
        param_grid = {
                    #'objective': ['binary:logitraw'],
                    'objective': ['rank:pairwise'],
                    #'booster': ['gblinear'],
                    'n_estimators': [580],

                    'max_depth': [6],
                    'min_child_weight': [45],
                    'gamma': [0.],

                    'subsample': [0.85],
                    'colsample_bytree': [0.65],

                    'learning_rate': [0.045],
                    }
        for k, v in cv_grid.items():
            param_grid[k] = v

        from sklearn.cross_validation import StratifiedKFold
        from sklearn.grid_search import GridSearchCV
        grid = GridSearchCV(estimator=clf,
                                param_grid=param_grid,
                                cv=StratifiedKFold(train_y, n_folds=nfolds),
                                scoring='roc_auc',
                                n_jobs=1,
                                verbose=2,
                                refit=False)
        grid.fit(train_X, train_y)
        print('grid scores:')
        for item in grid.grid_scores_:
            print('  {:s}'.format(item))
        print('best score: {:.5f}'.format(grid.best_score_))
        print('best params:', grid.best_params_)

    """ Print feature importances """
    if (False
        #or True
        ):
        clf.fit(train_X, train_y)
        feature_names = train_X.columns.values.tolist()
        from numpy import zeros
        feature_importances = zeros(len(feature_names))
        importances = clf.booster().get_fscore()
        for i, feat in enumerate(feature_names):
            if feat in importances:
                feature_importances[i] += importances[feat]
                pass
            pass
        import operator
        sorted_importances = sorted(zip(feature_names, feature_importances), key=operator.itemgetter(1), reverse=True)
        for k, v in sorted_importances:
            print("{}\t{}".format(v, k))
            pass
        print([k for k, v in sorted_importances if v == 0])
        pass

    """ Hyperopt """
    if do_hyperopt:
        def objective(space):
            #param_grid = {'objective': ['binary:logistic']}
            param_grid = {'objective': ['binary:logitraw']}
            #param_grid = {'objective': ['rank:pairwise']}
            #param_grid = {'objective': ['rank:pairwise'], 'booster_type': ['gblinear']}
            for k, v in space.items():
                if k in ['n_estimators', 'max_depth', 'min_child_weight', 'num_pairwise']:
                    v = int(v)
                    pass
                param_grid[k] = [v]
                pass

            from sklearn.cross_validation import StratifiedKFold
            from sklearn.grid_search import GridSearchCV
            grid = GridSearchCV(estimator=clf,
                            param_grid=param_grid,
                            cv=StratifiedKFold(train_y, n_folds=nfolds),
                            scoring='roc_auc',
                            n_jobs=1,
                            verbose=2,
                            refit=False)
            grid.fit(train_X, train_y)

            print('best score: {:.5f}  best params: {}'.format(grid.best_score_, grid.best_params_))
            return -grid.best_score_

        from sys import path as sys_path
        sys_path.insert(0, './hyperopt')
        from hyperopt import fmin, tpe, hp

        # cheatsheet:
        # https://github.com/hyperopt/hyperopt/wiki/FMin#21-parameter-expressions
        space = {
            'n_estimators': hp.quniform("x_n_estimators", 500, 800, 10),
            'max_depth': hp.quniform("x_max_depth", 4, 8, 1),
            'min_child_weight': hp.quniform ('x_min_child', 45, 240, 5),
            'gamma': hp.uniform ('x_gamma', 0.0, 2.0),
            'scale_pos_weight': hp.uniform ('x_scale_pos_weight', 0.2, 1.0),

            #'num_pairsample': hp.quniform ('x_num_pairsample', 1, 4, 1),

            'subsample': hp.uniform ('x_subsample', 0.4, 1.0),
            'colsample_bytree': hp.uniform ('x_colsample_bytree', 0.4, 1.0)
            }
        best = fmin(fn=objective,
            space=space,
            algo=tpe.suggest,
            max_evals=400,
            )
        print(best)
        pass


    return



def main(argv=None): # IGNORE:C0111
    '''Command line options.'''
    from sys import argv as Argv

    if argv is None:
        argv = Argv
        pass
    else:
        Argv.extend(argv)
        pass

    from os.path import basename
    program_name = basename(Argv[0])
    program_version = "v%s" % __version__
    program_build_date = str(__updated__)
    program_version_message = '%%(prog)s %s (%s)' % (program_version, program_build_date)
    try:
        program_shortdesc = __import__('__main__').__doc__.split("\n")[1]
    except:
        program_shortdesc = __import__('__main__').__doc__
    program_license = '''%s

  Created by Wojciech Migda on %s.
  Copyright 2016 Wojciech Migda. All rights reserved.

  Licensed under the MIT License

  Distributed on an "AS IS" basis without warranties
  or conditions of any kind, either express or implied.

USAGE
''' % (program_shortdesc, str(__date__))

    try:
        from argparse import ArgumentParser
        from argparse import RawDescriptionHelpFormatter
        from argparse import FileType
        from sys import stdout

        # Setup argument parser
        parser = ArgumentParser(description=program_license, formatter_class=RawDescriptionHelpFormatter)

        parser.add_argument("-n", "--num-est",
            type=int, default=700, action='store', dest="nest",
            help="number of Random Forest estimators")

        parser.add_argument("-j", "--jobs",
            type=int, default=-1, action='store', dest="njobs",
            help="number of jobs")

        parser.add_argument("-f", "--cv-fold",
            type=int, default=5, action='store', dest="nfolds",
            help="number of cross-validation folds")

        parser.add_argument("--clf-params",
            type=str, default="{}", action='store', dest="clf_params",
            help="classifier parameters subset to override defaults")

        parser.add_argument("-G", "--cv-grid",
            type=str, default="{}", action='store', dest="cv_grid",
            help="cross-validation grid params (used if NFOLDS > 0)")

        parser.add_argument("-E", "--estimator",
            action='store', dest="estimator", default='XGBClassifier',
            type=str,# choices=['mean', 'median', 'most_frequent'],
            help="Estimator class to use")

        parser.add_argument("-H", "--hyperopt",
            action='store_true', dest="do_hyperopt",
            help="Do hyperopt exploration")


        # Process arguments
        args = parser.parse_args()

        for k, v in args.__dict__.items():
            print(str(k) + ' => ' + str(v))
            pass

        work(args.estimator,
             args.nest,
             args.njobs,
             args.nfolds,
             eval(args.cv_grid),
             eval(args.clf_params),
             args.do_hyperopt)


        return 0
    except KeyboardInterrupt:
        ### handle keyboard interrupt ###
        return 0
    except Exception as e:
        if DEBUG:
            raise(e)
            pass
        indent = len(program_name) * " "
        from sys import stderr
        stderr.write(program_name + ": " + repr(e) + "\n")
        stderr.write(indent + "  for help use --help")
        return 2

    pass


if __name__ == "__main__":
    if DEBUG:
        from sys import argv
        argv.append("-n 700")
        argv.append("--minimizer=Powell")
        argv.append("--clf-params={'learning_rate': 0.05, 'min_child_weight': 240, 'subsample': 0.9, 'colsample_bytree': 0.67, 'max_depth': 6, 'initial_params': [0.1, -1, -2, -1, -0.8, 0.02, 0.8, 1]}")
        argv.append("-f 10")
        pass
    from sys import exit as Exit
    Exit(main())
    pass
import logging

from Config import Config
from DataHandling.DataBaseHandler import DataBaseHandler, DataBaseHandlerDbConnectionException
from Io.Hive.HiveAggregator import HiveAggregator

if __name__ == '__main__':
    logging.basicConfig(filename='ContestAggregatorBot.log', level=logging.INFO)

    try:
        db = DataBaseHandler(Config.dbHost, Config.dbName, Config.dbUsername, Config.dbPassword)
    except DataBaseHandlerDbConnectionException as e:
        logging.error(e.message if hasattr(e, 'message') else 'Database connection error.')
        exit(1)

    try:
        ha = HiveAggregator(Config.accountToListen)
    except Exception as e:
        logging.error(e.message if hasattr(e, 'message') else 'Error fetching Hive posts.')
        exit(1)

    contests = ha.fetchContests()
    for contest in contests.values():
        db.addContest(contest)

    db.commit()

    logging.info('Found {amount} contests in the Hive.'.format(amount=len(contests)))
    logging.info('Added {amount} new contests.'.format(amount=db.getAmountOfAddedContests()))
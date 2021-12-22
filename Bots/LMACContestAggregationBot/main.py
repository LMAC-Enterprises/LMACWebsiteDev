import logging

from Configuration import Configuration
from DataHandling.DataHandler import DataHandler
from networking.HivePostAggregator import HivePostAggregator

if __name__ == '__main__':
    logging.basicConfig(
        filename='LMACContestAggregation.log',
        level=logging.INFO,
        format='%(asctime)s - %(levelname)s : %(message)s',
        datefmt='%m/%d/%Y %I:%M:%S %p'
    )
    logger = logging.getLogger()

    hpa = HivePostAggregator(Configuration.searchInAccount, Configuration.searchInMaxPosts)

    contests = hpa.fetchContests(Configuration.finalistMinimumReputation, Configuration.finalistMinimumHP)
    if contests is None:
        logger.info('No contests found.')
        exit(1)

    dataHandler = DataHandler(
        Configuration.mysqlServerAddress,
        Configuration.mysqlUser,
        Configuration.mysqlPassword,
        Configuration.mysqlDatabaseName
    )
    dataHandler.updateContests(contests)


import mysql.connector

from DataHandling.Entities.ContestEntity import ContestEntity
from DataHandling.Entities.WinnerEntity import WinnerEntity


class DataBaseHandlerDbConnectionException(Exception):
    pass


"""
Author: QuantumG
Description: Objects of this class are used to read data from and write data to the database.
"""


class DataBaseHandler:

    """
    CTor.
    Parameter (dbHost): Hostname of the database server.
    Parameter (dbName): Name of the database.
    Parameter (dbUsername): Username to get access to the database.
    Parameter (dbPassword): Password of the database user.
    """
    def __init__(self, dbHost, dbName, dbUsername, dbPassword):
        try:
            self.mysqlDb = mysql.connector.connect(
                host=dbHost,
                user=dbUsername,
                password=dbPassword,
                database=dbName
            )
        except Exception as e:
            raise DataBaseHandlerDbConnectionException(
                'Can\'t connect to the database. Please check your network connection and user credentials.')

        self.contestsLookup = {}
        self.newContestData = {}
        self.amountOfAddedContests = 0

        self.loadContests()

    """
    Loads all available contests from database into a lookup dictionary.
    Returns: Nothing.
    """
    def loadContests(self):
        cursor = self.mysqlDb.cursor(dictionary=True)
        cursor.execute("SELECT * FROM lmacrounds")

        roundsQueryResult = cursor.fetchall()
        for contestRound in roundsQueryResult:
            contest = ContestEntity()
            contest.contestId = int(contestRound['roundid'])
            contest.templateImageUrl = contestRound['templateimageurl']
            contest.announcementPostUrl = contestRound['announcementurl']
            contest.pollPostUrl = contestRound['pollurl']
            contest.winnersPostUrl = contestRound['winnersurl']
            contest.announcementPostTitle = contestRound['announcementtitle']
            contest.pollPostTitle = contestRound['polltitle']
            contest.winnersPostTitle = contestRound['winnerstitle']

            self.contestsLookup[contestRound['roundid']] = contest

            cursor.execute("SELECT * FROM lmacwinners WHERE round=%(round)s", {'round': contest.contestId})
            winnersQueryResult = cursor.fetchall()
            for contestWinner in winnersQueryResult:
                winner = WinnerEntity()
                winner.postUrl = contestWinner['posturl']
                winner.postTitle = contestWinner['title']
                winner.imageUrl = contestWinner['imageurl']
                winner.artist = contestWinner['artist']
                winner.round = int(contestWinner['round'])
                winner.winnerId = contestWinner['winnerid']
                winner.place = contestWinner['place']
                contest.winners.append(winner)

    """
    Adds a new contest to the commit list.
    Parameter (contest): A ContestEntity object.
    Returns: Nothing.
    """
    def addContest(self, contest: ContestEntity):
        self.newContestData[contest.contestId] = contest

    """
    Commits all changes to the database.
    Returns: Nothing.
    """
    def commit(self):
        for contest in self.newContestData.values():
            if contest.commit(self.mysqlDb, self.contestsLookup):
                self.amountOfAddedContests += 1

        self.mysqlDb.commit()

    """
    Returns the amount of newly added contests.
    Returns: The amount of newly added contests.
    """
    def getAmountOfAddedContests(self):
        return self.amountOfAddedContests
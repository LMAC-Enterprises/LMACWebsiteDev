import logging
import mysql.connector
import json


class DataHandler:
    COLUMN_CONTEST_ID = 0

    def __init__(self, mysqlServerAddress: str, mysqlUser: str, mysqlPassword: str, mysqlDatabase: str):

        self.logger = logging.getLogger()

        try:
            self.mysqlDb = mysql.connector.connect(
                host=mysqlServerAddress,
                user=mysqlUser,
                password=mysqlPassword,
                database=mysqlDatabase
            )
        except Exception as e:
            self.logger.error(
                'Database error. {originalMessage}'.format(originalMessage=getattr(e, 'message', repr(e))))

        self._contestLookup = {}
        self._loadContestIds()

    def _loadContestIds(self):
        cursor = self.mysqlDb.cursor()

        cursor.execute("SELECT * FROM lmac_contests ORDER BY contestId DESC LIMIT 1000")

        qresult = cursor.fetchall()
        for contestRow in qresult:
            self._contestLookup[contestRow[DataHandler.COLUMN_CONTEST_ID]] = True

    def _getJsonFromWinnersDict(self, winners: dict):
        output = {}
        for artist, entity in winners.items():
            output.update({artist: entity.tojson()})

        return json.dumps(output)

    def updateContests(self, contests: dict):
        cursor = self.mysqlDb.cursor()
        print('saving')
        for contestEntity in contests.values():
            if int(contestEntity.contestId) in self._contestLookup.keys():
                continue

            cursor.execute(
                "INSERT INTO lmac_contests (contestId, title, postUrl, templateImageUrl, winners) VALUES (%s, %s, %s, %s, %s)",
                (
                    contestEntity.contestId,
                    contestEntity.title,
                    contestEntity.postUrl,
                    contestEntity.templateImageUrl,
                    self._getJsonFromWinnersDict(contestEntity.winners)
                ))

        self.mysqlDb.commit()

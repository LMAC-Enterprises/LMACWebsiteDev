"""
Author: QuantumG
Description: Objects of this class represent winner data in a contest.
"""


class WinnerEntity:
    def __init__(self):
        self.postUrl = ''
        self.postTitle = ''
        self.imageUrl = ''
        self.artist = ''
        self.place = 0
        self.winnerId = 0
        self.round = 0

    def __str__(self):
        return self.postTitle + "\n" + self.postUrl + "\n" + self.imageUrl + '\n@' + self.artist + '\n\n'

    """
    Commits the entity object to the database if necessary.
    Parameter (dbCursor): A mysql-connector database cursor.
    Parameter (roundId): The number of the contest round.
    Parameter (lookup): A lookup dictionary of ContestEntity objects.
    Returns: Nothing.
    """
    def commit(self, dbCursor, roundId, lookup):
        if roundId not in lookup.keys():
            return

        dbCursor.execute('DELETE FROM lmacwinners WHERE round=%(round)s AND artist=%(artist)s',
                         {'round': roundId, 'artist': self.artist})
        dbCursor.execute(
            "INSERT INTO lmacwinners(round, place, artist, imageurl, posturl, title) VALUES(%(round)s, %(place)s, %(artist)s, %(imageurl)s, %(posturl)s, %(title)s)",
            {'round': roundId, 'place': self.place, 'artist': self.artist, 'imageurl': self.imageUrl,
             'posturl': self.postUrl, 'title': self.postTitle})

"""
Author: QuantumG
Description: Objects of this class represent data of a contest.
"""


class ContestEntity:
    def __init__(self):
        self.contestId = 0
        self.templateImageUrl = ''
        self.announcementPostUrl = ''
        self.pollPostUrl = ''
        self.winnersPostUrl = ''
        self.announcementPostTitle = ''
        self.pollPostTitle = ''
        self.winnersPostTitle = ''
        self.winners = []

    """
    Commits the entity object to the database if necessary.
    Parameter (dbConnection): A mysql-connector connection object.
    Parameter (lookup): A lookup dictionary of ContestEntity objects.
    Returns: True if the contest was committed newly.
    """
    def commit(self, dbConnection, lookup):

        dbCursor = dbConnection.cursor()

        addedNewly = False

        if self.contestId not in lookup.keys():
            dbCursor.execute(
                "INSERT INTO lmacrounds (roundid, winnerstitle, winnersurl, polltitle, pollurl, announcementtitle, announcementurl, templateimageurl) VALUES(%(roundid)s, %(winnerstitle)s, %(winnersurl)s, %(polltitle)s, %(pollurl)s, %(announcementtitle)s, %(announcementurl)s, %(templateimageurl)s)",
                {'roundid': self.contestId, 'winnerstitle': self.winnersPostTitle, 'winnersurl': self.winnersPostUrl,
                 'polltitle': self.pollPostTitle, 'pollurl': self.pollPostUrl,
                 'announcementtitle': self.announcementPostTitle, 'announcementurl': self.announcementPostUrl,
                 'templateimageurl': self.templateImageUrl})
            addedNewly = True
        else:
            dbCursor.execute(
                "UPDATE lmacrounds SET winnerstitle=%(winnerstitle)s, winnersurl=%(winnersurl)s, polltitle=%(polltitle)s, pollurl=%(pollurl)s, announcementtitle=%(announcementtitle)s, announcementurl=%(announcementurl)s, templateimageurl=%(templateimageurl)s WHERE roundid=%(roundid)s",
                {'roundid': self.contestId,
                 'winnerstitle': self.winnersPostTitle if len(self.winnersPostTitle) > 0 else lookup[
                     self.contestId].winnersPostTitle,
                 'winnersurl': self.winnersPostUrl if len(self.winnersPostUrl) > 0 else lookup[
                     self.contestId].winnersPostUrl,
                 'polltitle': self.pollPostTitle if len(self.pollPostTitle) > 0 else lookup[
                     self.contestId].pollPostTitle,
                 'pollurl': self.pollPostUrl if len(self.pollPostUrl) > 0 else lookup[self.contestId].pollPostUrl,
                 'announcementtitle': self.announcementPostTitle if len(self.announcementPostTitle) > 0 else lookup[
                     self.contestId].announcementPostTitle,
                 'announcementurl': self.announcementPostUrl if len(self.announcementPostUrl) > 0 else lookup[
                     self.contestId].announcementPostUrl,
                 'templateimageurl': self.templateImageUrl if len(self.templateImageUrl) > 0 else lookup[
                     self.contestId].templateImageUrl,
                 }
            )

        lookup[self.contestId] = self

        for winner in self.winners:
            winner.commit(dbCursor, self.contestId, lookup)

        return addedNewly

    def __str__(self):

        winnerStr = ''
        for winner in self.winners:
            winnerStr += str(winner) + '\n---\n'

        return str(self.contestId) + '#\n' + \
               'Template: ' + self.templateImageUrl + '\n' + \
               'Announcement post url: ' + self.announcementPostUrl + '\n' + \
               'Poll post url: ' + self.pollPostUrl + '\n' + \
               'Winners post url: ' + self.winnersPostUrl + '\n' + \
               'Announcement post title: ' + self.announcementPostTitle + '\n' + \
               'Poll post title: ' + self.pollPostTitle + '\n' + \
               'Winners post title: ' + self.winnersPostTitle + '\n' + \
               'Winners:\n' + winnerStr

import json
import re

from bs4 import BeautifulSoup

from beem.discussions import Query, Discussions_by_author_before_date

from DataHandling.Entities.ContestEntity import ContestEntity
from DataHandling.Entities.WinnerEntity import WinnerEntity

"""
Author: QuantumG
Description: Objects of this class are used to read contest data from Hive posts.
"""


class HiveAggregator:
    POST_TYPE_UNKNOWN = 0
    POST_TYPE_CONTEST_ANNOUNCEMENT = 1
    POST_TYPE_CONTEST_POLL = 2
    POST_TYPE_CONTEST_WINNERS = 3

    accountToListen: str

    """
    CTor.
    Parameter (accountToListen): The Hive account name which does the contest announcements.    
    """
    def __init__(self, accountToListen):
        self.accountToListen = accountToListen

    """
    Retrieves the type of a official contest Hive post by parsing the related permlink. (Announcement, Poll, Winners)
    Parameter (permlink): The permlink of a contest post.
    Returns: The type of the contest round.
    """
    def _getPostType(self, permlink):
        if 'let-s-make-a-collage' in permlink or 'lets-make-a-collage' in permlink or 'lmac' in permlink:
            if 'contest' in permlink and 'round' in permlink:
                return HiveAggregator.POST_TYPE_CONTEST_ANNOUNCEMENT

            if 'final-poll' in permlink and 'round' in permlink:
                return HiveAggregator.POST_TYPE_CONTEST_POLL

            if 'winner' in permlink and 'announcement' in permlink and 'round' in permlink:
                return HiveAggregator.POST_TYPE_CONTEST_WINNERS

        return HiveAggregator.POST_TYPE_UNKNOWN

    """
    Retrieves the contest Id (contest round number) by parsing a contest Hive post Url.
    Parameter (permlink): The permlink of a contest post.
    Returns: The number of the contest round.
    """
    def _getPostContestId(self, permlink):
        contestId = -1

        matches = re.search(r"round-(\d+)-", permlink)
        if matches and len(matches.groups()) > 0:
            return matches.group(1)

        return int(contestId)

    """
    Detects and retrieves a template image Url from list of Urls that was found in a contest announcement Hive post.
    Parameter (imageUrl): The imageEntity to update.
    Returns: The image Url of the contest template.
    """
    def getTemplateImageUrl(self, imageUrls):

        return imageUrls[2] if len(imageUrls) > 2 else (imageUrls[1] if len(imageUrls) > 1 else imageUrls[0])

    """
    Retrieves a place number by parsing a table row from a winner announcement Hive post.
    Parameter (contest): A HTML table row string.
    Returns: A winner place number.
    """
    def getWinnerPlace(self, content):

        if 'First' in content:
            return 1

        if 'Second' in content:
            return 2

        if 'Third' in content:
            return 3

        if 'Fourth' in content:
            return 4

        if 'Fifth' in content:
            return 5

        if 'Sixth' in content:
            return 6

        if 'Seventh' in content:
            return 7

        return 10

    """
    Retrieves all winners from a winner announcement Hive post.
    Parameter (body): A body text from a Hive post.
    Returns: An array of WinnerEntity objects.
    """
    def getWinners(self, body):

        winners = []

        for tableMatch in re.finditer(r"(<table>.*<\/table>)", body, re.DOTALL):
            table = tableMatch.group(0)
            soup = BeautifulSoup(table, features="html.parser")

            for linkedImage in soup.find_all('a'):
                winner = WinnerEntity()

                winner.postUrl = linkedImage['href']
                winner.imageUrl = linkedImage.contents[0]['src']
                winner.imageUrl = re.sub(r'^https://images.hive.blog/\d+x\d+/', '', winner.imageUrl)

                placeColumn = linkedImage.find_parent('tr').select_one(":nth-child(1)")
                winner.place = self.getWinnerPlace(str(placeColumn))

                matches = re.search(r"\/@(.*?)\/", winner.postUrl, re.DOTALL)
                if matches:
                    winner.artist = matches.group(1)
                    if winner.artist == 'shaka':
                        continue

                matches = re.search(r"\/@.*?\/(.*?)$", winner.postUrl, re.DOTALL)
                if matches:
                    winner.postTitle = str(matches.group(1)).replace('-', ' ').title()

                winners.append(winner)

            break

        return winners

    """
    Retrieves contests from Hive
    Returns: A dictionary of ContestEntity objects and round numbers as keys.
    """
    def fetchContests(self):

        contestEntities = {}

        for post in Discussions_by_author_before_date(limit=100, author=self.accountToListen):

            if not post.is_main_post():
                continue

            postType = self._getPostType(post.permlink)

            if postType == HiveAggregator.POST_TYPE_UNKNOWN:
                continue

            contestId = self._getPostContestId(post.permlink)
            if contestId == -1:
                continue

            if contestId not in contestEntities.keys():
                contestEntities[contestId] = ContestEntity()
                contestEntities[contestId].contestId = int(contestId)

            if postType == HiveAggregator.POST_TYPE_CONTEST_ANNOUNCEMENT:
                contestEntities[contestId].templateImageUrl = self.getTemplateImageUrl(post.json_metadata['image'])
                contestEntities[contestId].announcementPostTitle = post.title
                contestEntities[
                    contestId].announcementPostUrl = '/' + post.category + '/@' + post.author + '/' + post.permlink
                continue

            if postType == HiveAggregator.POST_TYPE_CONTEST_POLL:
                contestEntities[contestId].pollPostTitle = post.title
                contestEntities[contestId].pollPostUrl = '/' + post.category + '/@' + post.author + '/' + post.permlink
                continue

            if postType == HiveAggregator.POST_TYPE_CONTEST_WINNERS:
                contestEntities[contestId].winnersPostTitle = post.title
                contestEntities[
                    contestId].winnersPostUrl = '/' + post.category + '/@' + post.author + '/' + post.permlink
                contestEntities[contestId].winners = self.getWinners(post.body)
                continue

        return contestEntities

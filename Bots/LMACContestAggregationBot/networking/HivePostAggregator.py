import re
import logging
import ssl
import urllib
from urllib.error import URLError

from beem.discussions import Query, Discussions_by_author_before_date

from interpreting.DpollInterpreter import DpollInterpreter
from interpreting.LMACPollPostBodyInterpreter import LMACPollPostBodyInterpreter
from networking.ContestEntity import ContestEntity

"""
Author: QuantumG
Description: Objects of this class are used to read contest data from Hive posts.
"""


class HivePostAggregator:
    POST_TYPE_UNKNOWN = 0
    POST_TYPE_CONTEST_ANNOUNCEMENT = 1
    POST_TYPE_CONTEST_POLL = 2
    POST_TYPE_CONTEST_WINNERS = 3

    accountToListen: str

    """
    CTor.
    Parameter (accountToListen): The Hive account name which does the contest announcements.    
    """

    def __init__(self, accountToListen, searchInMaxPosts):
        self.accountToListen = accountToListen
        self.logger = logging.getLogger()
        self.sslContext = ssl.SSLContext()
        self.searchInMaxPosts = searchInMaxPosts

    def _fetchDpollAudit(self, author: str, permlink: str):
        relativeHiveUrl = '@{hiveAuthor}/{hivePermlink}'.format(hiveAuthor=author, hivePermlink=permlink)
        url = "https://dpoll.io/detail/{relativeUrl}/?audit=1".format(relativeUrl=relativeHiveUrl)

        try:
            pollContents = urllib.request.urlopen(url, context=self.sslContext).read()
        except URLError as e:
            return None

        return pollContents.decode("utf-8")

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
    Retrieves contests from Hive
    Returns: A dictionary of ContestEntity objects and round numbers as keys.
    """

    def fetchContests(self, minReputation, minHivePower):

        contestEntities = {}

        for post in Discussions_by_author_before_date(limit=self.searchInMaxPosts, author=self.accountToListen, tag="letsmakeacollage"):
            if not post.is_main_post():
                continue

            if 'final poll' not in post.title.lower():
                continue

            contestId = self._getPostContestId(post.permlink)
            if contestId == -1:
                self.logger.warning("Unknown contest Id in @{author}/{permlink}.".format(
                    author=post.author, permlink=post.permlink))
                continue

            dpollAudit = self._fetchDpollAudit(post.author, post.permlink)
            if dpollAudit is None:
                self.logger.warning("Couldn't load Dpoll audit @{author}/{permlink}.".format(
                    author=post.author, permlink=post.permlink))
                continue

            dpollInterpreter = DpollInterpreter(dpollAudit, minReputation, minHivePower)
            if len(dpollInterpreter.getPoll()) == 0:
                self.logger.warning("Couldn't interpret Dpoll audit @{author}/{permlink}.".format(
                    author=post.author, permlink=post.permlink))
                continue

            try:
                lppi = LMACPollPostBodyInterpreter(post.body, dpollInterpreter)
            except ValueError:
                self.logger.warning("Couldn't interpret poll post @{author}/{permlink}.".format(
                    author=post.author, permlink=post.permlink))
                continue

            contestEntity = ContestEntity(
                contestId,
                post.title,
                lppi.getTemplateImageUrl(),
                '@{author}/{permlink}'.format(
                    author=post.author,
                    permlink=post.permlink
                )
            )
            contestEntity.winners = lppi.getEntries()

            self.logger.info('Found and added @{author}/{permlink}.'.format(
                author=post.author, permlink=post.permlink))
            contestEntities[contestId] = contestEntity

        return contestEntities

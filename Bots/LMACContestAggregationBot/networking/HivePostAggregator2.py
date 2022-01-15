import logging
import re
import ssl

from beem.discussions import Discussions_by_author_before_date

from interpreting.WinnerPostInterpreter import WinnerPostInterpreter
from networking.ContestEntity import ContestEntity


class HivePostAggregator2:
    def __init__(self, accountToListen, searchInMaxPosts):
        self.accountToListen = accountToListen
        self.logger = logging.getLogger()
        self.sslContext = ssl.SSLContext()
        self.searchInMaxPosts = searchInMaxPosts

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
    def fetchContests(self):

        contestEntities = {}

        for post in Discussions_by_author_before_date(
                limit=self.searchInMaxPosts,
                author=self.accountToListen,
                tag="letsmakeacollage"
        ):
            if not post.is_main_post():
                continue

            if 'winner announcement' not in post.title.lower():
                continue

            contestId = self._getPostContestId(post.permlink)
            if contestId == -1:
                self.logger.warning("Unknown contest Id in @{author}/{permlink}.".format(
                    author=post.author, permlink=post.permlink))
                continue

            wpi = WinnerPostInterpreter(post.body)

            contestEntity = ContestEntity(
                contestId,
                post.title,
                wpi.templateImageUrl,
                '@{author}/{permlink}'.format(
                    author=post.author,
                    permlink=post.permlink
                )
            )
            contestEntity.winners = wpi.getWinners()

            self.logger.info('Found and added @{author}/{permlink}.'.format(
                author=post.author, permlink=post.permlink))
            contestEntities[contestId] = contestEntity

        return contestEntities

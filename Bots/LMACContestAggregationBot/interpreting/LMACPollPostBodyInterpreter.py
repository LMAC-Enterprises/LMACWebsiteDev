import logging
import re

from interpreting.ContestWinnerEntity import ContestWinnerEntity
from interpreting.DpollInterpreter import DpollInterpreter


class LMACPollPostBodyInterpreter:
    ENTRY_ROW_REGEX = r'---\s+@([a-z-A-Z0-9_\-]+): \[-> Original post\]\((.*)\)\s+!\[\]\((.*)\)\s+'
    TEMPLATE_URL_REGEX = r'Template Image\s+\[\-> Original post\]\(.*\)\s+!\[\]\((.*)\)'
    ENTRY_COL_AUTHOR = 1
    ENTRY_COL_POST_URL = 2
    ENTRY_COL_IMAGE_URL = 3
    ENTRY_COL_TEMPLATE_URL = 1

    def __init__(self, bodyContent: str, dpollInterpreter: DpollInterpreter):
        self._dpollInterpreter = dpollInterpreter
        self._entries = {}
        self._templateImageUrl = ''
        self._logger = logging.getLogger()

        self._performInterpretation(bodyContent)

    def _performInterpretation(self, bodyContent):
        matches = re.finditer(self.ENTRY_ROW_REGEX, bodyContent, re.MULTILINE)

        for matchNum, match in enumerate(matches, start=1):
            author = match.group(self.ENTRY_COL_AUTHOR)
            if author not in self._dpollInterpreter.poll.keys():
                self._logger.warning('{author} not found in DPoll data but found in Final Poll post.'.format(author=author))
                continue

            winner = self._dpollInterpreter.poll[author]

            winner.postUrl = match.group(self.ENTRY_COL_POST_URL)
            winner.imageUrl = match.group(self.ENTRY_COL_IMAGE_URL)
            self._entries[match.group(self.ENTRY_COL_AUTHOR)] = winner

        matches = re.finditer(self.TEMPLATE_URL_REGEX, bodyContent, re.MULTILINE)
        for matchNum, match in enumerate(matches, start=1):
            self._templateImageUrl = match.group(self.ENTRY_COL_TEMPLATE_URL)
            break

    def getEntries(self):
        return self._entries

    def getTemplateImageUrl(self):
        return self._templateImageUrl

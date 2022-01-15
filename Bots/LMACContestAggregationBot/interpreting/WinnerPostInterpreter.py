import re

from interpreting.ContestWinnerEntity import ContestWinnerEntity


class WinnerPostInterpreter:
    _placeTerms: dict = {'first place': 1, 'second place': 2, 'third place': 3, 'fourth place': 4, 'fifth place': 5}
    _topTerms: dict = {'top 10': 10, 'top 15': 15}
    _tableControllers: dict = {}
    _top5WinnerRegex = r"<a href=\"(.*)\".*<img\s*src=\"(.*)\".*@([a-z0-9_]*)"
    _topXXWinnerRegex = r"<a href=\"(.*)\".*<img\s*src=\"(.*)\".*@([a-z0-9_]*)"
    _templateImageRegex = r"<img\s+src=\"(.*)\""
    _winnerEntities: dict
    templateImageUrl: str

    def __init__(self, hivePostBody: str):
        self._winnerEntities= {}
        self.templateImageUrl = ''
        self._interprete(hivePostBody)

    def _findTop5(self, hivePostBody: str):
        winnerEntities = {}

        for placeTerm in self._placeTerms.keys():
            maxIterationCountdown = 5
            while True:
                try:
                    index = hivePostBody.lower().index(placeTerm) + len(placeTerm)
                    hivePostBody = hivePostBody[index:]
                except ValueError:
                    break

                matches = re.search(self._top5WinnerRegex, hivePostBody, re.MULTILINE)
                if not matches:
                    continue

                winnerEntities[matches.group(3)] = ContestWinnerEntity(
                    self._placeTerms[placeTerm],
                    matches.group(3),
                    matches.group(2).replace('https://images.hive.blog/0x0/', ''),
                    matches.group(1),
                )

                maxIterationCountdown -= 1
                if maxIterationCountdown <= 0:
                    break

        self._winnerEntities.update(winnerEntities)

    def _findTopXX(self, hivePostBody):
        winnerEntities = {}

        for topTerm in self._topTerms.keys():
            try:
                index = hivePostBody.lower().index(topTerm)
                hivePostBody = hivePostBody[index:]
            except ValueError:
                continue
            matches = re.finditer(self._topXXWinnerRegex, hivePostBody, re.MULTILINE)
            if not matches:
                continue

            entryCountdown = 5
            for matchNum, match in enumerate(matches, start=1):
                winnerEntities[match.group(3)] = ContestWinnerEntity(
                    self._topTerms[topTerm],
                    match.group(3),
                    match.group(2).replace('https://images.hive.blog/0x0/', ''),
                    match.group(1),
                )
                entryCountdown -= 1
                if entryCountdown == 0:
                    break

        self._winnerEntities.update(winnerEntities)

    def _findTemplateImageUrl(self, hivePostBody: str):
        try:
            index = hivePostBody.index('Template image')
            hivePostBody = hivePostBody[index:]
        except ValueError:
            return

        matches = re.search(self._templateImageRegex, hivePostBody, re.MULTILINE)
        if not matches:
            return

        self.templateImageUrl = matches.group(1).replace('https://images.hive.blog/0x0/', '')

    def _interprete(self, hivePostBody: str):
        self._findTop5(hivePostBody)
        self._findTopXX(hivePostBody)
        self._findTemplateImageUrl(hivePostBody)

    def getWinners(self):
        return self._winnerEntities

from interpreting.ContestWinnerEntity import ContestWinnerEntity


class DpollInterpreter:
    COLUMN_INDEX_CHOICE = 0
    COLUMN_INDEX_VOTER = 1
    COLUMN_INDEX_TRANSACTION_ID = 2
    COLUMN_INDEX_BLOCK_NUM = 3
    COLUMN_INDEX_REPUTATION = 4
    COLUMN_INDEX_HIVE_POWER = 5
    COLUMN_INDEX_POST_COUNT = 6
    COLUMN_INDEX_ACCOUNT_AGE = 7

    def __init__(self, dpollData: str, minReputation, minHivePower):

        dpollData = dpollData.replace('<pre>', '')
        dpollData = dpollData.replace('</pre>', '')

        if dpollData[0] != '+':
            raise ValueError('Error. This is not a dpoll audit result.')

        self.minReputation = minReputation
        self.minHivePower = minHivePower
        self.poll = {}
        self.interprete(dpollData)

    def interprete(self, data):
        lines = data.split('\n')

        # Skip data header
        lines.pop(0)
        lines.pop(0)
        lines.pop(0)

        votings = {}
        votingCount = 0

        for line in lines:
            if line[0] == '+':
                continue

            columns = line[1:].split('|')
            voter = columns[self.COLUMN_INDEX_VOTER].strip()
            choice = columns[self.COLUMN_INDEX_CHOICE].strip()
            hivePower = int(columns[self.COLUMN_INDEX_HIVE_POWER].strip())
            reputation = float(columns[self.COLUMN_INDEX_REPUTATION].strip())

            if hivePower < self.minHivePower or reputation < self.minReputation:
                continue

            if voter not in votings:
                votings[voter] = []

            if choice in votings[voter]:
                continue

            votingCount += 1
            votings[voter].append(choice)

        votesPerChoice = {}
        percPerChoice = {}
        for votingChoices in votings.values():
            for votingChoice in votingChoices:
                if votingChoice not in votesPerChoice:
                    votesPerChoice[votingChoice] = 0

                votesPerChoice[votingChoice] += 1

        sortedVotesPerChoice = sorted(votesPerChoice.items(), key=lambda x: x[1], reverse=True)

        place = 1
        winners = []
        winnersAssoc = {}
        for artist, votes in sortedVotesPerChoice:
            artist = artist[1:]

            if len(winners) > 1 and votes == winners[-1]['votes']:
                place = winners[-1]['place']
            winners.append({
                'artist': artist,
                'votes': votes,
                'place': place
            })
            winnersAssoc[artist] = ContestWinnerEntity(place, artist, "", "")
            place += 1
        print(winnersAssoc)
        self.poll = winnersAssoc

    def getPoll(self) -> ContestWinnerEntity:
        return self.poll

class ContestEntity:
    contestId: int
    title: str
    templateImageUrl: str
    postUrl: str

    winners: dict

    def __init__(self, contestId: int, title: str, templateImageUrl: str, postUrl: str):
        self.contestId = contestId
        self.title = title
        self.templateImageUrl = templateImageUrl
        self.postUrl = postUrl

    def __str__(self):
        return str({
            'contestId': self.contestId,
            'title': self.title,
            'templateImageUrl': self.templateImageUrl,
            'postUrl': self.postUrl,
        })


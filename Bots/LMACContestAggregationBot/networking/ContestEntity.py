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



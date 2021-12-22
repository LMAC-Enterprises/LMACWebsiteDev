class DefaultConfiguration:
    finalistMinimumReputation: int = 49
    finalistMinimumHP: int = 250

    mysqlServerAddress: str = ''
    mysqlUser: str = ''
    mysqlPassword: str = ''
    mysqlDatabase: str = ''

    searchInAccount: str = ''


class Configuration(DefaultConfiguration):
    mysqlHost = "localhost"
    mysqlUser = "testuser"
    mysqlPassword = "testpassword"
    mysqlDatabaseName = "mydatabase"

    searchInAccount = 'shaka'

    searchInMaxPosts = 5


from pydantic import BaseSettings


class CommonSettings(BaseSettings):
    APP_NAME: str = "PSI - Z41"
    DEBUG_MODE: bool = True


class ServerSettings(BaseSettings):
    HOST: str = "0.0.0.0"
    PORT: int = 8000


class DatabaseSettings(BaseSettings):
    # TODO: add default values for docker?
    DB_URL: str
    DB_NAME: str


class Settings(CommonSettings, ServerSettings, DatabaseSettings):
    class Config:
        env_file = '.env', '.env.prod'


settings = Settings()

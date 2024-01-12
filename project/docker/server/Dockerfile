FROM python:3.10

WORKDIR /server

COPY --from=project_top_dir . project

RUN chmod 777 ./project/docker/server/run_server.sh

RUN apt-get install -y python3

RUN python3 -m pip install pydantic-settings
RUN python3 -m pip install -r ./project/requirements.txt

CMD ["sleep", "infinity"]

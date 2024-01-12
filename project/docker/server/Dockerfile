FROM python:3.10

WORKDIR /server

COPY --from=project_top_dir . project

# RUN chmod 777 ./docker/server/run_server.py

RUN apt-get install -y python3

EXPOSE 8888/udp
EXPOSE 8889/udp

CMD ["sleep", "infinity"]

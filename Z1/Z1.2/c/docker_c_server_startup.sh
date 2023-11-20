set -e

NAME='z41_z_1_c_server'

if [[ "$(docker ps | grep $NAME)" ]]; then
    docker stop $NAME
fi

if [[ "$(docker ps -aqf name=$NAME)" ]]; then
    docker rm $NAME
fi

docker build --tag $NAME:v1 .

docker create -it --name $NAME --network z41_network --cap-add NET_ADMIN $NAME:v1

docker ps -a

docker start -ai $NAME
FROM debian:bullseye

RUN apt update -y && apt upgrade -y

RUN apt install build-essential -y

WORKDIR "/ft_irc"

CMD ["make", "r"]
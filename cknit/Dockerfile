FROM python:3-alpine

WORKDIR /app
#COPY code /app/cknit/
COPY run.py /app/run.py

#RUN echo "https://mirrors.aliyun.com/alpine/latest-stable/main/" > /etc/apk/repositories \
#    && echo "https://mirrors.aliyun.com/alpine/latest-stable/community/" >> /etc/apk/repositories
#RUN apk update && apk add --no-cache build-base cmake
#RUN apk update && apk add git

#RUN su root
#RUN chmod +x /usr/bin/cc
RUN cd /app/cknit && mkdir build && cd build && cmake .. &&  sudo make && sudo make install

CMD ["python3", "run.py"]
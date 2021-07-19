FROM alpine:3.13
RUN apk add cmatrix
CMD [ "cmatrix" ]

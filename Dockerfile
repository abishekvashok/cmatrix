FROM alpine:3.13
RUN apk add cmatrix
CMD [ "cmatrix" ]

#### Litle doc ####
# Now with command `docker build -t <NameOfImage:Tag> .` An image will create that the main work is to run Cmatrix 
# After creation with command `docker run -it <NameOfImage:Tag>` Cmatrix will run :)

#### For @abishekvashok ####
# at first wanted to install from source code but can't autoconf in alpine and this way is neater 

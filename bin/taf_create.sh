
APP=${1}
SRV=${2}

#创建文件
#会覆盖 ${APP}/${SRV}/${SRV}Servant.jce

/usr/local/taf/create_taf_server.sh  ${APP}  ${SRV}Server  ${SRV}Servant

#编辑jce文件
#根据jce文件创建接口文件
cd ${APP}/${SRV}Server/
vim ${SRV}Servant.jce
/usr/local/taf/bin/jce2cpp   ${SRV}Servant.jce

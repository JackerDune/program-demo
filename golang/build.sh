
if [ ! -f go.mod ];then
    go mod init main
fi

go mod tidy
go build

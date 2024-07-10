```bash
brew install googletest

```


```bash
brew install nlohmann-json
brew install boost

```


```bash
g++ -std=c++11 server.cpp -o server -I/opt/homebrew/include -L/opt/homebrew/lib -lboost_system -lboost_thread
```

## 构建和推送客户端镜像：
```bash
# 构建客户端镜像
docker build -t sheldonhh/me-client -f Dockerfile.client .

# 推送客户端镜像
docker push sheldonhh/me-client

```

## 构建和推送服务器镜像：
```bash
# 构建服务器镜像
docker build -t sheldonhh/me-server -f Dockerfile.server .

# 推送服务器镜像
docker push sheldonhh/me-server
```
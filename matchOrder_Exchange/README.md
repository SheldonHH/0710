```bash
brew install googletest

```



```bash
find / -name "mysql_driver.h"
find / -name "libmysqlcppconn*"

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


# windows k8s
使用 Scoop 安装 kubectl：

powershell
Copy code
scoop install kubectl
验证安装：

powershell
Copy code
kubectl version --client

```bash
# 部署服务器
kubectl apply -f server-deployment.yaml

# 部署客户端
kubectl apply -f client-deployment.yaml
```

5. 验证部署
您可以使用以下命令查看部署和服务的状态：
```bash
# 查看服务器部署状态
kubectl get deployments

# 查看客户端部署状态
kubectl get deployments

# 查看服务状态
kubectl get services
```

```bash
PS C:\Users\xphua\0710\matchOrder_Exchange> kubectl get deployments
>>
NAME                READY   UP-TO-DATE   AVAILABLE   AGE
client-deployment   0/100   100          0           62s
server-deployment   0/3     3            0           62s
PS C:\Users\xphua\0710\matchOrder_Exchange> kubectl get services
>>
NAME             TYPE           CLUSTER-IP       EXTERNAL-IP   PORT(S)        AGE
kubernetes       ClusterIP      10.96.0.1        <none>        443/TCP        5m41s
server-service   LoadBalancer   10.104.136.174   <pending>     80:31784/TCP   69s
```

```bash

C:\Users\xphua\0710>kubectl create clusterrolebinding dashboard-admin-sa --clusterrole=cluster-admin --serviceaccount=default:dashboard-admin-sa
clusterrolebinding.rbac.authorization.k8s.io/dashboard-admin-sa created

C:\Users\xphua\0710>
C:\Users\xphua\0710>kubectl get secrets
No resources found in default namespace.
```




1. 查看 Pod 状态
您可以使用 kubectl 命令查看所有 Pod 的状态：
```bash
kubectl get pods -o wide
```
2. 查看 Deployment 状态
使用以下命令查看 Deployment 的状态：


```bash
手动创建 secret 并绑定服务账户
创建服务账户和 secret

手动创建一个 secret 并将其绑定到服务账户：

sh
Copy code
kubectl create secret generic dashboard-admin-sa-token --from-literal=token=$(kubectl create token dashboard-admin-sa)
kubectl annotate serviceaccount dashboard-admin-sa kubernetes.io/service-account.name=dashboard-admin-sa
kubectl annotate secret dashboard-admin-sa-token kubernetes.io/service-account.name=dashboard-admin-sa
获取 secret

获取 dashboard-admin-sa-token secret 的名称：

sh
Copy code
kubectl get secret
然后获取该 secret 的 token：

sh
Copy code
kubectl describe secret dashboard-admin-sa-token
```
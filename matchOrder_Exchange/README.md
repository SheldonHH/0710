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



```bash

kubectl delete pods --all
kubectl delete deployments --all
# 部署服务器
kubectl apply -f server-deployment.yaml

# 部署客户端
kubectl apply -f client-deployment.yaml
```
## 删除全部
```bash
kubectl delete pods --all
kubectl delete deployments --all
```

```bash
# 进入检查网络
kubectl run -it --rm debug --image=busybox --restart=Never -- sh

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
minikube stop
minikube start --cpus 4 --memory 3000
```


## GUI
```bash
kubectl apply -f https://raw.githubusercontent.com/kubernetes/dashboard/v2.2.0/aio/deploy/recommended.yaml
```

# 我是mac
```bash
cat <<EOF | kubectl apply -f -
apiVersion: v1
kind: ServiceAccount
metadata:
  name: dashboard-adminuser
  namespace: kubernetes-dashboard
EOF

# serviceaccount/dashboard-adminuser created

cat <<EOF | kubectl apply -f -
apiVersion: rbac.authorization.k8s.io/v1
kind: ClusterRoleBinding
metadata:
  name: dashboard-adminuser
roleRef:
  apiGroup: rbac.authorization.k8s.io
  kind: ClusterRole
  name: cluster-admin
subjects:
- kind: ServiceAccount
  name: dashboard-adminuser
  namespace: kubernetes-dashboard
EOF

kubectl -n kubernetes-dashboard create token dashboard-adminuser

# eyJhbGciOiJSUzI1NiIsImtpZCI6InltRWFHVTY3X3dtcXk2b0w5Si11M0liWTJ2ZzVKVFFwLTUweUJsNVBHUEUifQ.eyJhdWQiOlsiaHR0cHM6Ly9rdWJlcm5ldGVzLmRlZmF1bHQuc3ZjLmNsdXN0ZXIubG9jYWwiXSwiZXhwIjoxNzIwNjczMjUxLCJpYXQiOjE3MjA2Njk2NTEsImlzcyI6Imh0dHBzOi8va3ViZXJuZXRlcy5kZWZhdWx0LnN2Yy5jbHVzdGVyLmxvY2FsIiwianRpIjoiMDNlM2YzMmMtOTdjYS00M2NjLTllODMtMWU1ZGYxOGU3MDMxIiwia3ViZXJuZXRlcy5pbyI6eyJuYW1lc3BhY2UiOiJrdWJlcm5ldGVzLWRhc2hib2FyZCIsInNlcnZpY2VhY2NvdW50Ijp7Im5hbWUiOiJkYXNoYm9hcmQtYWRtaW51c2VyIiwidWlkIjoiOTlhZDlmMjktZjRlYi00ODU3LThlMGUtODRjZjk5MGM2ZDE5In19LCJuYmYiOjE3MjA2Njk2NTEsInN1YiI6InN5c3RlbTpzZXJ2aWNlYWNjb3VudDprdWJlcm5ldGVzLWRhc2hib2FyZDpkYXNoYm9hcmQtYWRtaW51c2VyIn0.bk-Q-h24soJo5I2Vv3PUV_3Y960dJSGR1qKQYwZihwdJAg_fV4OhVNH17csC8sN9eSSASlI51ByP0nHVMCidfAgEdQY-0iRvPVwSIYPXywbOUuYOdNS8oRWdI5xP2V5kedyqNDLyhYJ-00ra5ZxY3MTWOJuezNHHM7neYeajPN4dOfYo_E0dWSE67XKDucd_wiU9kgOVHeS1fdAMrk3ZjIGEhv7hvJeelXZkJ3aeqTg4RxLc3Z5bXM_I4XWVKtPHOOtneyqOTfNN21UvxiiKOkeLwM8cAcsmTtOMQtg9LnbHYQ7UZnbBF1CTaWOJ1TDE8t9khjSbCBaobg7fHYVZ6w

kubectl proxy
```

```bash


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
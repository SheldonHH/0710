
在 Kubernetes 中，使用 127.0.0.1 只能连接到同一 Pod 内的服务。如果客户端和服务器在不同的 Pod 中，使用 127.0.0.1 是无法连接到服务器的。相反，我们需要使用服务的 DNS 名称来确保客户端可以连接到 Kubernetes 集群中的服务。

为什么使用 SERVER_ADDRESS
Pod 的隔离性：Kubernetes 中每个 Pod 都有自己的网络命名空间，127.0.0.1 只在当前 Pod 内部有效。如果客户端和服务器在不同的 Pod 中，客户端无法通过 127.0.0.1 连接到服务器。

服务发现：Kubernetes 提供了服务发现的机制，通过服务的 DNS 名称可以在集群内部进行服务间的通信。例如，server-service.default.svc.cluster.local 是 server-service 服务在 default 命名空间内的完全限定域名。

动态 IP：Pod 的 IP 地址是动态的，可能会随着 Pod 的重启而变化。服务的 DNS 名称是固定的，通过服务名称可以稳定地访问到相应的服务。
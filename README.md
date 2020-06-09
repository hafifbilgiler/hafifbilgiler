# hafifbilgiler
--------------------------General Codes------------------------------------------

Nginx Pod Create:
    
    kubectl create -f nginx-pod.yaml

Nginx ReplicaSet Create:
    
    kubectl create -f nginx-replicaset.yaml

Nginx Deployment Create:
    
    kubectl create -f nginx-deployment.yaml


Service Create For Nginx Deployment:

    kubectl create -f nginx-service.yaml

 
Job Create:

    kubectl create -f job.yaml   

Job Create For Backoff Limits:

    kubectl create -f job-backoff.yaml 

Job Create For Completions And Parallelism:

    kubectl create -f job-completions-parallelism.yaml

Job Create For TTL After Finished:

    kubectl create -f job-ttl.yaml


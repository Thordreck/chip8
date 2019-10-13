pipeline {
    agent { docker { image 'ubuntu:xenial' } }
    stages {
        stage('build') {
            steps {
                sh 'gcc --version'
            }
        }
    }
}

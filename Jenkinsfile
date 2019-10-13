pipeline {
    agent { docker { image 'ubuntu:xenial' } }
    stages {
        stage('build') {
            steps {
		sh 'sudo add-apt-repository ppa:ubuntu-toolchain-r/test'
                sh 'sudo apt update'
		sh 'sudo apt install g++-9'
                sh 'export CXX=/usr/bin/g++-9'
                sh 'mkdir -p build && cd build'
                sh 'cmake -DBUILD_TESTS=True ..'
                sh 'make'
            }
        }
        stage('Test') {
            steps {
                sh 'ctest --verbose'
            }
        }
    }
}

pipeline {
    agent { dockerfile true }
    stages {
        stage('build') {
            steps {
                sh 'mkdir -p ./build && cd ./build'
                sh 'cmake -DBUILD_TESTS=True ..'
                sh 'make'
            }
        }
        stage('test') {
            steps {
                sh 'ctest --verbose'
            }
        }
    }
}

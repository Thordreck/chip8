pipeline {
    agent { dockerfile true }
    stages {
        stage('build') {
            steps {
                sh 'mkdir -p ./build'
                dir("build") {
                    sh 'cmake -DBUILD_TESTS=True ..'
                    sh 'make'
                }
            }
        }
        stage('test') {
            steps {
                dir("build") {
                    sh 'ctest --verbose'
                }
            }
        }
    }
}

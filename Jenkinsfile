pipeline {
   agent { dockerfile true }

   stages {
      stage('Build') {
         steps {
            // Compile it
            sh '''
                mkdir -p build && cd build
                cmake -DCMAKE_CXX_COMPILER=g++-9 -DBUILD_INTERPRETER=1 -DBUILD_TESTS=1 ..
                make
            '''
         }
      }
      stage('Test') {
         steps {
            // Execute the tests
            sh 'ctest'
         }
      }
   }
}

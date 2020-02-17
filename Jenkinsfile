pipeline {
   agent { dockerfile true }

   stages {
      stage('Build') {
         steps {
            // Compile it
            sh '''
                mkdir -p build && cd build
                cmake -DBUILD_TESTS=1 ..
                make
            '''
         }
      }
   }
}

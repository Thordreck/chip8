pipeline {
   agent {
        dockerfile {
            args '''
                -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix
                -v /dev/snd:/dev/snd --privileged
                -v /dev/shm:/dev/shm \
                -v /etc/machine-id:/etc/machine-id \
                -v /run/user/$uid/pulse:/run/user/${uid}/pulse \
                -v /var/lib/dbus:/var/lib/dbus \
                -v ${HOME}/.pulse:/home/dev/.pulse
            '''
        }
   }

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
            sh '''
                cd build
                ./chip8_cpp_tests --gtest_output="xml"
            '''
         }

        post {
            always {
                // Import ctests results using xunit plugin
                xunit (
                    thresholds: [ skipped(failureThreshold: '0'), failed(failureThreshold: '0') ],
                    tools: [ GTest(pattern: 'test_detail.xml')]
                )
            }
         }
      }
   }

}

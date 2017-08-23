node {
  try {
    stage('checkout') {
      checkout scm
    }
    stage('prepare') {
      sh "git clean -fdx"
      sh "npm prune"
    }
    stage('compile') {
      sh "node -v"
      sh "npm install"
    }
    stage('test') {
      sh "npm test"
    }
  } finally {
    stage('cleanup') {
      echo "doing some cleanup..."
    }
  }
}
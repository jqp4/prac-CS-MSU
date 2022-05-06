# https://users.parallel.ru/wikiplus/pages/18-how-to-create-ssh-key
# user (user1): skryabin_gleb
# login (mgu):  edu-cmc-skpod21-323-07
# ssh-keygen -t rsa -b 2048 -C 'mgu-user1' -f ~/.ssh/mgu


ssh-keygen -t rsa -b 2048 -C 'edu-cmc-skpod21-323-07-skryabin_gleb' -f ~/.ssh/edu-cmc-skpod21-323-07


# gleb@MacBook-Air-Gleb-2 skipod % sh sshcommands.sh 
# Generating public/private rsa key pair.
# Enter passphrase (empty for no passphrase): 
# Enter same passphrase again: 
# Your identification has been saved in /Users/gleb/.ssh/edu-cmc-skpod21-323-07.
# Your public key has been saved in /Users/gleb/.ssh/edu-cmc-skpod21-323-07.pub.
# The key fingerprint is:
# SHA256:wFclWOUiRcfI0qpspG6k5l3mspSGvXD/+pGT+kjXU0I edu-cmc-skpod21-323-07-skryabin_gleb
# The key's randomart image is:
# +---[RSA 2048]----+
# |         **=+    |
# |     .  oo++.    |
# |      o ooE .    |
# |      .o.o .     |
# |     + .S . .    |
# |   oo.+  + o     |
# |  o+*.+ * o      |
# |  o*+B + o .     |
# | o..+o*=+        |
# +----[SHA256]-----+


ssh -i ~/.ssh/edu-cmc-skpod21-323-07 edu-cmc-skpod21-323-07@polus.cmc.msu.ru





brew install libomp
brew install llvm
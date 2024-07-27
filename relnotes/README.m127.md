Since m126:

* CentOS 7 EOL on 1 Jul 2024: small update to how we build the packages; We now more explicitly
  build for mac os 10.13+ in Skia.

* Adding references and code snipplets to OpenGL settings for Apple users. fixes #214

* GrVkExtensionFlags & GrVkFeatureFlags removed upsteam.

* Some redirections/aliases are added to allow newly written python codes to look like
  their current C counterparts (in addition to supporting a m87-like API).
  We also added some direct calls to currently APIs which were accessible
  only via m87-like emulations. Re-enabled a few parameterized tests removed
  during the m87->m116 transition.

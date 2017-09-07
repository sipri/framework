def VerifyCodeSign(file):
    import os
    import subprocess
    with open(os.devnull, 'w') as FNULL:
        import colorama
        colorama.init()
        if subprocess.call(["..\SignTool\signtool", "verify", "/pa", "{0}".format(file)], stdout=FNULL, stderr=FNULL):
            print(colorama.Fore.LIGHTRED_EX + "[  FAILED  ] {0}".format(file))
        else:
            print(colorama.Fore.LIGHTGREEN_EX + "[  PASSED  ] {0}".format(file))
        print(colorama.Style.RESET_ALL)

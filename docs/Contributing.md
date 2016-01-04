# Contributing to TemperFine
## Introduction
---------------
Outside assistance in creating TemperFine is highly encouraged. You are invited to pick an [Active Task] (./docs/ActiveTasks.md), perform the task, and create a pull request with your changes -- see the **Pull Request** section below.

Frequent contributers will be granted direct commit access to TemperFine.

## Development Environment
--------------------------
TemperFine is developed with CodeBlocks. You should download and install CodeBlocks from [their website](http://www.codeblocks.org/downloads/26), selecting the release with MinGW (GCC-TDM 2.7.1) for Windows.

To simplify dependency management, assets; library binaries (DLL, LIB); and includes are included within the TemperFine repository. Therefore, once you have downloaded and installed CodeBlocks, **you can build and run TemperFine immediately**, no additional setup required.

If you want to build TemperFine with a different toolset (Visual Studio), compiler version (!= GCC 2.7.1), and/or OS (OS != Windows 7-10), you will need to replace the library binaries with the appropriate binaries for your setup.

## Managing Source Code
------------------------
TemperFine uses Git for source control.
Unlike other projects, TemperFine uses a single official branch, **master** for main development work and release. This means that:
* Individual commits should be *small*
* Individual commits should be *descriptive*
* **master** must always build.
* **master** force pushes are disallowed.

You can develop in a side branch as desired (required if using pull requests).

## Pull Request
---------------

Pull requests should contain:
* Your active task (partially completed is ok)
* An updated [Active Tasks] (./ActiveTasks.md) page.
* An updated [LICENSE] (../LICENSE) file (only for your first contribution).

Once your pull request has been approved, congradulations! You have successfully contributed to a public open-source project, helping to improve software available to all.

## Licensing
------------
TemperFine is licensed under the [MIT LICENSE] (../LICENSE) license. Any external contributions will also be under that license, which is why your first contribution *must* also include you updating the license file.

If you want to add an external dependency to TemperFine, that dependency's license must be compatible with TemperFine's license. **Of particular concern**, LGPL/GPL dependencies must be dynamically-linked to TemperFine, if used.
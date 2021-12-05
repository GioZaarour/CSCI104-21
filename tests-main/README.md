# Tests

Tests for homework in Curricula format.
For assignments whose deadlines have not passed yet, only a partial grading artifact is provided.

## How To Run

In order to run tests, you'll have to install `curricula` on your virtual machine.
To do so, run the following commands:

```
python3.9 -m pip install curricula-grade-cpp curricula-grade curricula-format
```

After doing so, you can run and format your report with the following command:

```
python3.9 -m curricula grade run -g <hw> <submission> -d <destination> --format
```

The arguments captured in angle brackets should be interpolated as follows:

- `<hw>` should be the path to the corresponding homework's grading artifact, i.e. `./hw4`
- `<submission>` should be the path to the submitted code for the particular assignment.
- `<destination>` should be where you want the report details and formatted report to end up.
  For this option, you can simply provide `.`, which will dump the reports in the current working directory.
  
For batch grading, simply include multiple submission paths.
Additional flags may be of use in this scenario; you can see the full list by running `python3.9 -m curricula grade run help`.

- `--progress` shows progress during batch grading.
- `--summarize` shows overall statistics for submissions graded in a batch.
- `--tasks` can allow you to filter grading tasks to actuall run.

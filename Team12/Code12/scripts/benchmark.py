# run the tests one by one as per script
# for each script, save its results in this dict
# dict: {test_name: []}

from subprocess import run, DEVNULL
import os
import xml.etree.ElementTree as ET
from statistics import mean, stdev

num_times = 10

rootdir = (
    run(["git", "rev-parse", "--show-toplevel"], capture_output=True)
    .stdout.decode()
    .strip()
)
builddir = os.path.join(rootdir, "Team12/Code12/build")
testdir = os.path.join(rootdir, "Team12/Tests12")

autotester = os.path.join(builddir, "src/autotester/autotester")
outdir = os.path.join(rootdir, "Team12/Code12/tests/output")

testWrapper = os.path.join(rootdir, "Team12/Code12/src/autotester/src/TestWrapper.cpp")
sourcefiles = [f for f in os.listdir(testdir) if "_source.txt" in f]
sourceQueryPair = [
    (sf, sf.replace("source", "queries").replace("Source", "Queries"))
    for sf in sourcefiles
]


def extractRuntime(outfile, statsfile):
    xmlRoot = ET.parse(outfile).getiterator("query")
    times = [float(query.find("time_taken").text) for query in xmlRoot]
    print(times, file=statsfile)


def autotest(sf, qf, statsfile):
    testDesc = sf.replace("Source_source", "_output").replace(".txt", "")
    outfile = os.path.join(outdir, f"{testDesc}.xml")
    sourcefile, queryfile = os.path.join(testdir, sf), os.path.join(testdir, qf)
    run([autotester, sourcefile, queryfile, outfile], stdout=DEVNULL)
    print(sf, file=statsfile)
    extractRuntime(outfile, statsfile)


def build():
    currdir = os.curdir
    run(["cmake", "--build", builddir, "--", "-j6"], stdout=DEVNULL)
    print("Finished building.")


def turn_on_optimiser(optimise):
    run(
        [
            "sed",
            "-Ei",
            f"s/ui, .*\\)/ui, {'true' if optimise else 'false'}\\)/",
            testWrapper,
        ],
        stdout=DEVNULL,
    )


def run_tests(optimise):
    turn_on_optimiser(optimise)
    build()
    statsfilename = "optim.pyraw" if optimise else "no-optim.pyraw"
    with open(statsfilename, "w") as statsfile:
        for _ in range(num_times):
            print(f"Round {_+1} ...")
            for sf, qf in sourceQueryPair:
                autotest(sf, qf, statsfile)
        print("done.")


def run_analysis():
    optim_dict = {sf: [] for sf in sourcefiles}
    no_optim_dict = {sf: [] for sf in sourcefiles}
    with open("optim.pyraw", "r") as optim:
        curr_entry = []
        for line in optim:
            if line[0] != "[":  # is filename
                curr_entry = optim_dict[line.strip()]
            else:
                curr_entry.append(eval(line))
    with open("no-optim.pyraw", "r") as no_optim:
        curr_entry = []
        for line in no_optim:
            if line[0] != "[":  # is filename
                curr_entry = no_optim_dict[line.strip()]
            else:
                curr_entry.append(eval(line))

    # for each source file, print out avg speedup percent, min change, max change
    for sf in sourcefiles:
        noop_op = [
            list(pair for pair in zip(noop_run, op_run) if pair[0] > 10)
            for (noop_run, op_run) in zip(no_optim_dict[sf], optim_dict[sf])
        ]
        if any(not run for run in noop_op):
            continue
        no_optim_times = [pair[0] for run in noop_op for pair in run]
        optim_times = [pair[1] for run in noop_op for pair in run]
        # print(no_optim_times, optim_times)
        # no_optim_times = [sum(run) for run in no_optim_dict[sf]]
        # optim_times = [sum(run) for run in optim_dict[sf]]
        abs_change = [a - b for (a, b) in zip(optim_times, no_optim_times)]
        avg_change_percent = mean(abs_change) / mean(no_optim_times) * 100
        max_change_percent = (
            (max(optim_times) - min(no_optim_times)) / mean(no_optim_times) * 100
        )
        min_change_percent = (
            (min(optim_times) - max(no_optim_times)) / mean(no_optim_times) * 100
        )
        print(
            f"{sf:52}, avg: {avg_change_percent:5.1f}%, max: {max_change_percent:5.1f}%, min: {min_change_percent:5.1f}%"
        )


def __main__():
    # print(f"Running benchmarks for {num_times} rounds...")
    # print("Optimizer on")
    # run_tests(True)
    # print("Optimizer off")
    # run_tests(False)
    # print("Calculating results")
    run_analysis()


__main__()

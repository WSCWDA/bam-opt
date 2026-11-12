# BFS BaM I/O Trace

Enable the opt-in per-level trace with `-I true` and `-m 6` (BAFS_DIRECT). For example:

```bash
./bfs -f /path/to/graph.bel -m 6 -v 3 -I true -p 4096
```

`-v 3` selects `BASELINE_PC`, where edge-list reads use `seq_read()`. The program writes CSV records prefixed with `BFS_IO_TRACE` to stdout; other benchmark output may appear in the same stream. Filter those records when importing the trace:

```bash
./bfs -f /path/to/graph.bel -m 6 -v 3 -I true -p 4096 2>&1 | grep '^BFS_IO_TRACE' > bfs_io_trace.csv
```

The trace reports one row per BFS level and BaM range:

- `warp_page_groups`: total unique (range, BaM page) groups across active lanes for each dynamic `seq_read` warp instruction.
- `warps_with_N_pages`: histogram of dynamic warp instructions that touched N distinct BaM pages (1–32). This is a distribution, not per-warp identity.
- `nvme_read_commands`: actual page-read submissions counted at the cache miss path.
- `nvme_read_bytes`: command count × configured BaM page size. This is the requested BaM read payload, not a device-side measurement of NAND traffic.
- `miss_lane_accesses` and `miss_edge_demand_bytes_est`: lane accesses and their estimated EdgeT demand associated with cache misses. This is not unique useful bytes within each fetched page.
- `hit_lane_accesses`: lane accesses served by already resident BaM pages. It does not identify whether reuse came from another warp or an earlier BFS level.

The opt-in mode copies/resets counters after each level and adds atomic updates to the warp coalescing path. Use it for characterization, not for timing comparisons. Exact per-page useful-byte utilization and cross-warp/cross-level reuse attribution require page-identity events or a per-page access bitmap; this minimal trace does not claim those metrics.

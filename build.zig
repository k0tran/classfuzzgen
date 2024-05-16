const std = @import("std");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const cfgen = b.addExecutable(.{
        .name = "cfgen",
        .target = target,
        .optimize = optimize,
    });
    cfgen.addCSourceFiles(.{
        .files = &[_][]const u8{
            "src/main.cpp",
            "src/ast.cpp",
            "src/class.cpp",
            "src/cli.cpp",
            "src/render.cpp",
        },
    });
    cfgen.linkLibCpp();
    cfgen.addIncludePath(.{ .path = "include/" });
    cfgen.linkSystemLibrary("clang");
    b.installArtifact(cfgen);

    const run = b.addRunArtifact(cfgen);
    run.step.dependOn(b.getInstallStep());
    if (b.args) |args|
        run.addArgs(args);

    const run_step = b.step("run", "Run cfgen");
    run_step.dependOn(&run.step);
}

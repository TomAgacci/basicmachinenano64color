void BasicVM::presentColor() {
    const auto &monoFb = video.getFramebuffer();
    color.mapMonoToColor(monoFb);
}

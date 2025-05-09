#include "src/array.h"
#include "src/stream.h"

namespace linalg {

mx::array Norm(const mx::array& a,
               std::optional<std::variant<float, std::string>> ord,
               std::optional<std::variant<int, std::vector<int>>> optional_axis,
               std::optional<bool> optional_keepdims,
               mx::StreamOrDevice s) {
  std::optional<std::vector<int>> axis;
  if (optional_axis)
    axis = PutIntoVector(std::move(*optional_axis));
  bool keepdims = optional_keepdims.value_or(false);

  if (!ord) {
    return mx::linalg::norm(a, std::move(axis), keepdims, s);
  } else if (auto str = std::get_if<std::string>(&ord.value()); str) {
    return mx::linalg::norm(a, std::move(*str), std::move(axis), keepdims, s);
  } else {
    return mx::linalg::norm(a, std::get<float>(*ord), std::move(axis), keepdims,
                            s);
  }
}

auto Svd(const mx::array& a, ki::Arguments* args) {
  auto compute_uv = args->TryGetNext<bool>().value_or(true);
  auto s = args->TryGetNext<mx::StreamOrDevice>().value_or(std::monostate());
  return mx::linalg::svd(a, compute_uv, s);
}

auto Cross(const mx::array& a,
           const mx::array& b,
           std::optional<int> axis,
           mx::StreamOrDevice s) {
  return mx::linalg::cross(a, b, axis.value_or(-1), s);
}

}  // namespace linalg

void InitLinalg(napi_env env, napi_value exports) {
  napi_value linalg = ki::CreateObject(env);
  ki::Set(env, exports, "linalg", linalg);

  ki::Set(env, linalg,
          "norm", &linalg::Norm,
          "qr", &mx::linalg::qr,
          "svd", &linalg::Svd,
          "inv", &mx::linalg::inv,
          "triInv", &mx::linalg::tri_inv,
          "cholesky", &mx::linalg::cholesky,
          "choleskyInv", &mx::linalg::cholesky_inv,
          "pinv", &mx::linalg::pinv,
          "cross", &linalg::Cross,
          "eigvalsh", &mx::linalg::eigvalsh,
          "eigh", &mx::linalg::eigvalsh,
          "lu", &mx::linalg::lu,
          "luFactor", &mx::linalg::lu_factor,
          "solve", &mx::linalg::solve,
          "solveTriangular", &mx::linalg::solve_triangular);
}
